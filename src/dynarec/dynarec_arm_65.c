#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <errno.h>

#include "debug.h"
#include "box86context.h"
#include "dynarec.h"
#include "emu/x86emu_private.h"
#include "emu/x86run_private.h"
#include "x86run.h"
#include "x86emu.h"
#include "box86stack.h"
#include "callback.h"
#include "emu/x86run_private.h"
#include "x86trace.h"
#include "dynarec_arm.h"
#include "dynarec_arm_private.h"
#include "arm_printer.h"

#include "dynarec_arm_helper.h"

uintptr_t dynarecGS(dynarec_arm_t* dyn, uintptr_t addr, uintptr_t ip, int ninst, int* ok, int* need_epilog)
{
    uint8_t opcode = F8;
    uint8_t nextop;
    int32_t i32;
    uint32_t u32;
    uint8_t gd, ed;
    uint8_t wback;
    int fixedaddress;
    switch(opcode) {
        case 0x03:
            INST_NAME("ADD Gd, GS:Ed");
            SETFLAGS(X_ALL, SF_SET);
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop = F8;
            GETGD;
            GETEDO(x12);
            emit_add32(dyn, ninst, gd, ed, x3, x12);
            break;

        case 0x2B:
            INST_NAME("SUB Gd, GS:Ed");
            SETFLAGS(X_ALL, SF_SET);
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop = F8;
            GETGD;
            GETEDO(x12);
            emit_sub32(dyn, ninst, gd, ed, x3, x12);
            break;

        case 0x33:
            INST_NAME("XOR Gd, GS:Ed");
            SETFLAGS(X_ALL, SF_SET);
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop = F8;
            GETGD;
            GETEDO(x12);
            emit_xor32(dyn, ninst, gd, ed, x3, x12);
            break;

        case 0x3B:
            INST_NAME("CMP Gd, GS:Ed");
            SETFLAGS(X_ALL, SF_SET);
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop = F8;
            GETGD;
            GETEDO(x12);
            emit_cmp32(dyn, ninst, gd, ed, x3, x12);
            break;

        case 0x89:
            INST_NAME("MOV GS:Ed, Gd");
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop=F8;
            GETGD;
            if((nextop&0xC0)==0xC0) {   // reg <= reg
                MOV_REG(xEAX+(nextop&7), gd);
            } else {                    // mem <= reg
                addr = geted(dyn, addr, ninst, nextop, &ed, x2, &fixedaddress, 0, 0);
                STR_REG_LSL_IMM5(gd, ed, x12, 0);
            }
            break;

        case 0x8B:
            INST_NAME("MOV Gd, GS:Ed");
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop=F8;
            GETGD;
            if((nextop&0xC0)==0xC0) {   // reg <= reg
                MOV_REG(gd, xEAX+(nextop&7));
            } else {                    // mem <= reg
                addr = geted(dyn, addr, ninst, nextop, &ed, x2, &fixedaddress, 0, 0);
                LDR_REG_LSL_IMM5(gd, ed, x12, 0);
            }
            break;

        case 0x8F:
            INST_NAME("POP GS:Ed");
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop = F8;
            if((nextop&0xC0)==0xC0) {
                POP(xESP, (1<<(xEAX+(nextop&7))));
            } else {
                addr = geted(dyn, addr, ninst, nextop, &ed, x1, &fixedaddress, 0, 0);
                POP(xESP, (1<<x2));
                STR_REG_LSL_IMM5(x2, ed, x12, 0);
            }
            break;

        case 0xA1:
            INST_NAME("MOV EAX, GS:Id");
            grab_tlsdata(dyn, addr, ninst, x1);
            i32 = F32S;
            if(i32>-4096 && i32<4096) {
                LDR_IMM9(xEAX, x1, i32);
            } else {
                MOV32(x2, i32);
                ADD_REG_LSL_IMM5(x1, x1, x2, 0);
                LDR_IMM9(xEAX, x1, 0);
            }
            break;

        case 0xA3:
            INST_NAME("MOV GS:Od, EAX");
            grab_tlsdata(dyn, addr, ninst, x1);
            u32 = F32;
            MOV32(x2, u32);
            ADD_REG_LSL_IMM5(x2, x1, x2, 0);
            STR_IMM9(xEAX, x2, 0);
            break;

        case 0xC7:
            INST_NAME("MOV GS:Ed, Id");
            grab_tlsdata(dyn, addr, ninst, x12);
            nextop=F8;
            if((nextop&0xC0)==0xC0) {   // reg <= i32
                i32 = F32S;
                ed = xEAX+(nextop&7);
                MOV32(ed, i32);
            } else {                    // mem <= i32
                addr = geted(dyn, addr, ninst, nextop, &ed, x2, &fixedaddress, 0, 0);
                i32 = F32S;
                MOV32(x3, i32);
                STR_REG_LSL_IMM5(x3, ed, x12, 0);
            }
            break;

        case 0xFF:
            nextop = F8;
            grab_tlsdata(dyn, addr, ninst, x12);
            switch((nextop>>3)&7) {
                case 6: // Push Ed
                    INST_NAME("PUSH GS:Ed");
                    if((nextop&0xC0)==0xC0) {   // reg
                        DEFAULT;
                    } else {                    // mem <= i32
                        addr = geted(dyn, addr, ninst, nextop, &ed, x2, &fixedaddress, 0, 0);
                        LDR_REG_LSL_IMM5(x3, ed, x12, 0);
                        PUSH(xESP, 1<<x3);
                    }
                    break;
                default:
                    DEFAULT;
            }
            break;

        default:
            DEFAULT;
    }
    return addr;
}

