#if defined(GO) && defined(GOM) && defined(GO2) && defined(DATA) && defined(END)

GOM(dladdr, iFEpp)
// dladdr1
GOM(dlclose, iFEp)
GOM(dlerror, pFE)
DATAB(_dlfcn_hook, 4)
// dlinfo
// dlmopen
GOM(dlopen, pFEpi)
GOM(dlsym, pFEpp)
GOM(dlvsym, pFEppp)   // Weak

END()

#else
#error Mmmm...
#endif