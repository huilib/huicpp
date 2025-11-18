
#ifndef __HUICPP_EXPORTS_H__
#define __HUICPP_EXPORTS_H__

#   if defined(_MSC_VER) && defined(_WIN32)
#define HUICPP_WIN
#define HUICPP_OS "WINDOWS"

#ifdef HUICPP_EXPORTS 
#   define HUICPP_LIB __declspec(dllexport)
#   else
#   define HUICPP_LIB __declspec(dllimport)
#endif 
#else 
#define HUICPP_OS "LINUX"
#define HUICPP_LIB
#endif //HUICPP_HAS_DLL

#endif // __HUICPP_EXPORTS_H__

