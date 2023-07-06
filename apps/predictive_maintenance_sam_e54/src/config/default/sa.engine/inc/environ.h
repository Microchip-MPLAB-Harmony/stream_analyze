/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2011-2016 Tore Risch, UDBL, SA
 *
 * Description: Macros to handle different compilation environments
 *
 ****************************************************************************/

#ifndef _environ_h_
#define _environ_h_
#define str_wrap_def(s) str_def(s)
#define str_def(s) #s

#ifdef PLATFORM_ENVIRON
#include str_wrap_def(PLATFORM_ENVIRON)
#else

#define PRINT_LF "\n"
//-----------------------------
#define HAS_POPEN 1
#if defined(__cplusplus)
#define EXTLANG extern "C"
#else
#define EXTLANG extern
#endif
//-----------------------------

#undef ARCHITECTURE_x86
#undef ARCHITECTURE_ARMA
#undef ARCHITECTURE_ARMB
#undef WORDLEN
#undef OS_Windows
#undef OS_Android
#undef OS_OSX

#ifdef __APPLE__
  #include <sys/uio.h>
  #include <unistd.h>
  #define OS_OSX 1
  #ifdef __x86_64__
    #define ARCHITECTURE_x86 1
    #define WORDLEN 64
  #elif defined(__aarch64__)
    #define ARCHITECTURE_ARMA 1
    #define WORDLEN 64
  #endif

#elif defined(_MSC_VER)
  #include <io.h>
  #define OS_Windows 1
  #if defined(_M_ARM)
    #define ARCHITECTURE_ARMB 1
    #define WORDLEN 32
  #elif defined(_M_ARM64)
    #define ARCHITECTURE_ARMA 1
    #define WORDLEN 64
  #else
    #define ARCHITECTURE_x86 1
    #if defined(_WIN64)
      #define WORDLEN 64
    #else
      #define WORDLEN 32
    #endif
  #endif

#elif defined(__GNUC__)
  #define O_BINARY 0
  #ifdef __ANDROID__
    #define OS_Android 1
  #elif defined(__LINUX__)
    #define OS_Linux 1
  #endif
  #ifdef __x86_64__
    #define ARCHITECTURE_x86 1
    #define WORDLEN 64
  #elif defined(__aarch64__)
    #define ARCHITECTURE_ARMA 1
    #define WORDLEN 64
  #elif defined(__arm__)
    #define ARCHITECTURE_ARMB 1
    #define WORDLEN 32
  #elif defined (__i386__)
    #define ARCHITECTURE_x86 1
    #define WORDLEN 32
  #elif defined(WASM)
    #define ARCHITECTURE_x86 1
    #define WORDLEN 64
  #else
    #error No architecture def
    #define ARCHITECTURE_x86 0
    #define WORDLEN 32
  #endif

//#elif defined(__clang__)

#else
#error "unknown compiler"
#endif
#if defined(WASM)
#include "wasm_environ.h"
#endif

#if defined(ESP)
  #include <ctype.h>
  #include <unistd.h>
  // Open file for binary read/write
  #define O_BINARY 0
  #undef HAS_POPEN
  #include <sys/signal.h>
  #include <unistd.h>
  #define signal(i, f)

#elif defined(XDK)
  #include "alloca.h"
  #include <ctype.h>
  #include <unistd.h>
  // Open file for binary read/write
  #define O_BINARY 0
  #undef HAS_POPEN
  #include <sys/signal.h>
  #include <unistd.h>

#endif

#ifdef OS_OSX
  #define FP LONGINT
#else
  #define FP int
#endif

//-----------------------------
#if defined(STATIC)
#define EXPORT
#define EXTERN extern
#elif defined(KERNEL_DLL) // Compiling kernel DLL
#define EXPORT __declspec(dllexport)
#define EXTERN EXPORT
#elif defined(_DLL) || defined(_USRDLL) // Compiling DLL on top of kernel DLL
#define EXPORT __declspec(dllexport)
#define EXTERN EXTLANG __declspec(dllimport)
#elif defined(WIN32) // Compiling non-DLL under Windows
#define EXPORT
#define EXTERN EXTLANG __declspec(dllimport)
#elif defined(LINUX)
#define EXPORT
#define EXTERN EXTLANG
#elif defined(ESP)
#define EXPORT
#define EXTERN EXTLANG
#elif defined(XDK)
#define EXPORT
#define EXTERN EXTLANG
#else
#error Unsupported operating system
#endif
//-----------------------------
#define LONGCHARS 30

#ifdef OS_Windows
// Win32
#include <malloc.h>
#define INLINE __inline
#define NOINLINE _declspec(noinline)
#define LONGINT __int64
#define LongIntToString(x, buff) sprintf(buff, "%I64d", x)
#define LONGINTMIN _I64_MIN
#define LONGINTMAX _I64_MAX
#define ATOI64 _atoi64
#ifdef _WIN64
#define X64 1
#endif
#define THREADLOCAL __declspec(thread)
#if _MSC_VER >= 1800
#define isatty(x) (x < 1)
#define fileno _fileno
#define strdup _strdup
#define getcwd _getcwd
#define read(fp, start, size) _read(fp, start, (unsigned int)size)
#define write(fp, start, size) _write(fp, start, (unsigned int)size)
#define open _open
#define close _close
#define popen _popen
#define pclose _pclose
#define PATH_MAX _MAX_PATH
#endif
#else // Unix
#include <unistd.h>
#define _getpid getpid
#define _unlink unlink
#define _chdir chdir
#define INLINE static inline
#define NOINLINE
#define LONGINT long long
#define LongIntToString(x, buff) sprintf(buff, "%lld", x)
#define LONGINTMIN LLONG_MIN
#define LONGINTMAX LLONG_MAX
#define ATOI64 atoll
#define STATIC_EXTENSIONS 1
#ifdef __ANDROID__
#include <strings.h>
#define THREADLOCAL __thread
#define S_IREAD S_IRUSR
#define S_IWRITE S_IWUSR
#define S_IEXEC S_IXUSR
#define STATIC_EXTENSIONS 1


#else
#define O_BINARY 0
#define THREADLOCAL __thread
#endif
#if UINTPTR_MAX == 0xffffffffffffffff
// 64 bits Unix
#define X64 1
#endif
#endif

#ifdef ESP
#undef THREADLOCAL
#define THREADLOCAL
//#include "esp_files.h"
#endif
#ifdef XDK
#undef THREADLOCAL
#define THREADLOCAL
#endif

//-----------------------------
#define OPENED_WITH_POPEN 2
#define add_slash_last(x) if(x[strlen(x)-1]!='/') strcat(x,"/")
#endif
#endif // PLATFORM_INCLUDE

#ifdef SAME54
#undef LongIntToString
#define LongIntToString(x, buff) sprintf(buff, "%ld", (long)x)
#endif


#ifdef MICROCORE
#define CORE 1
#endif
