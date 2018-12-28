#if defined(GO) && defined(GOM) && defined(GO2) && defined(DATA) && defined(END)

DATA(__data_start, 4)
DATA(_edata, 4)
// _fini
// IMG_Init
// IMG_InitJPG
// IMG_InitPNG
// IMG_InitTIF
// IMG_InitWEBP
// IMG_isBMP
// IMG_isCUR
// IMG_isGIF
// IMG_isICO
// IMG_isJPG
// IMG_isLBM
// IMG_isPCX
// IMG_isPNG
// IMG_isPNM
// IMG_isSVG
// IMG_isTIF
// IMG_isWEBP
// IMG_isXCF
// IMG_isXPM
// IMG_isXV
// IMG_Linked_Version
GO(IMG_Load, pFp)
// IMG_LoadBMP_RW
// IMG_LoadCUR_RW
// IMG_LoadGIF_RW
// IMG_LoadICO_RW
// IMG_LoadJPG_RW
// IMG_LoadLBM_RW
// IMG_LoadPCX_RW
// IMG_LoadPNG_RW
// IMG_LoadPNM_RW
// IMG_Load_RW
// IMG_LoadSVG_RW
// IMG_LoadTexture
// IMG_LoadTexture_RW
// IMG_LoadTextureTyped_RW
// IMG_LoadTGA_RW
// IMG_LoadTIF_RW
// IMG_LoadTyped_RW
// IMG_LoadWEBP_RW
// IMG_LoadXCF_RW
// IMG_LoadXPM_RW
// IMG_LoadXV_RW
GO(IMG_Quit, vFv)
// IMG_QuitJPG
// IMG_QuitPNG
// IMG_QuitTIF
// IMG_QuitWEBP
// IMG_ReadXPMFromArray
// IMG_SaveJPG
// IMG_SaveJPG_RW
// IMG_SavePNG
// IMG_SavePNG_RW
// _init
DATA(nsvg__colors, 4)
// nsvgCreateRasterizer
// nsvgDelete
// nsvgDeleteRasterizer
// nsvgParse
// nsvg__parseXML
// nsvgRasterize

END()

#else
#error Mmmm...
#endif