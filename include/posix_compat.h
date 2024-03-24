#pragma once
/*
 * Map low I/O functions for MS. This allows us to disable MS language
 * extensions for maximum portability.
 */
#ifdef WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#include <fcntl.h>
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define popen _popen
#define pclose _pclose
#define open _open
#define read _read
#define write _write
#define close _close
#define mkdir _mkdir
#define lseek _lseek
#if _MSC_VER <= 1200 /* Versions below VC++ 6 */
#define vsnprintf _vsnprintf
#endif
#define O_RDONLY _O_RDONLY
#define O_BINARY _O_BINARY
#define O_CREAT _O_CREAT
#define O_WRONLY _O_WRONLY
#define O_TRUNC _O_TRUNC
#define S_IREAD _S_IREAD
#define S_IWRITE _S_IWRITE
#define S_IFDIR _S_IFDIR
#define F_OK 0b000
#define R_OK 0b010
#define W_OK 0b100
#define DLL_API __declspec(dllexport)
#define mode_t int
#include <process.h>
#define getpid _getpid
#include <direct.h>
#define getcwd _getcwd
#endif
