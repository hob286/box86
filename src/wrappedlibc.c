#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define _GNU_SOURCE         /* See feature_test_macros(7) */
#include <dlfcn.h>

#include "wrappedlibs.h"

#include "debug.h"
#include "wrapper.h"
#include "bridge.h"
#include "library_private.h"
#include "x86emu.h"

int32_t my___libc_start_main(x86emu_t* emu, int *(main) (int, char * *, char * *), 
    int argc, char * * ubp_av, void (*init) (void), void (*fini) (void), 
    void (*rtld_fini) (void), void (* stack_end)); // implemented in x86run_private.c
uint32_t my_syscall(x86emu_t *emu); // implemented in x86syscall.c
void my___stack_chk_fail(x86emu_t* emu)
{
    StopEmu(emu, "Stack is corrupted, abborting");
}
void my___gmon_start__(x86emu_t *emu)
{
    printf_log(LOG_DEBUG, "__gmon_start__ called (dummy call)\n");
}

int wrappedlibc_init(library_t* lib)
{
    lib->priv.w.lib = dlopen("libc.so.6", RTLD_NOW);
    if(!lib->priv.w.lib) {
        return -1;
    }
    lib->priv.w.bridge = NewBridge();
    return 0;
}
void wrappedlibc_fini(library_t* lib)
{
    if(lib->priv.w.lib)
        dlclose(lib->priv.w.lib);
    lib->priv.w.lib = NULL;
    FreeBridge(&lib->priv.w.bridge);
}
int wrappedlibc_get(library_t* lib, const char* name, uintptr_t *offs, uint32_t *sz)
{
    uintptr_t addr = 0;
    uint32_t size = 0;
    void* symbol = NULL;

#include "wrappedlib_defines.h"
#include "wrappedlibc_private.h"
#include "wrappedlib_undefs.h"


    if(!addr)
        return 0;
    *offs = addr;
    *sz = size;
    return 1;
}
