//Author: Arno Bauernöppel
#ifndef IMPORT
#ifdef WIN32
#define IMPORT __cdecl(("extern")) __dllimport
#elif GNU_C
#define IMPORT __cdecl(("extern"))
#else
#define IMPORT extern "C"
#endif
