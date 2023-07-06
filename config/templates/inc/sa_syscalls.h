#ifndef _SA_SYSCALLS_H_
#define _SA_SYSCALLS_H_
#include "environ.h"
#include "sa_threads.h"

#ifdef OS_Windows
#define _WINSOCKAPI_ // stops windows.h including winsock.h
#include <windows.h>
#include <winsock2.h>
#include "c0xdir.h"
#include <io.h>
#include <stdlib.h>
#include <stdio.h>

#elif defined(ARMM4)
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#ifndef SAME54
#include <sys/unistd.h>
#endif
#include <sys/stat.h>
typedef void DIR;
struct dirent {
  int d_ino;
  char d_name[100];
};
#else

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#endif

#ifndef OPTIONAL_MC_FN
#define OPTIONAL_MC_FN
#endif

extern void sa_tzset(void) OPTIONAL_MC_FN;

extern int sa_sys_isdst() OPTIONAL_MC_FN;

/*  OS-specific setup */
EXTERN void sa_sys_init(void);

/*  Sleep for us microseconds. */
EXTERN void sa_sys_sleep(int us) OPTIONAL_MC_FN;

EXTERN sa_time_t sa_sys_now(void) OPTIONAL_MC_FN;

typedef struct timeval a_timeval_t;
/*  Get time of day. See
    https://man7.org/linux/man-pages/man2/settimeofday.2.html */
EXTERN void sa_sys_gettimevalofday(a_timeval_t *res) OPTIONAL_MC_FN;

/* Get a system millisecond clock. Does not need to be acutal time
   but needs to me a monotonically increasing clock.*/
EXTERN int sa_sys_msclock(void) OPTIONAL_MC_FN;

/* Unbuffered reading of count bytes from fd into buf
   see: https://linux.die.net/man/2/read*/
EXTERN unsigned int sa_sys_read(int fd, void *buf, size_t count) OPTIONAL_MC_FN;

/* Unbuffered writing of buf to fd
   see: https://linux.die.net/man/2/write*/
EXTERN unsigned int sa_sys_write(int fd, void *buf, size_t count) OPTIONAL_MC_FN;

/* Fill startupdirbuff with the path to the startupdir of sa.engine
   this is usually the bin folder under sa.engine but on embedded
   systems this might vary. */
EXTERN int sa_sys_getstartupdir(char *startupdirbuff, size_t buf_size) OPTIONAL_MC_FN;

EXTERN int sa_sys_mkdir(char *path) OPTIONAL_MC_FN;
EXTERN int sa_sys_chdir(const char *path) OPTIONAL_MC_FN;
EXTERN char *sa_sys_getcwd(char *buf, size_t size) OPTIONAL_MC_FN;
EXTERN char *sa_sys_getenv(const char *name) OPTIONAL_MC_FN;
EXTERN int sa_sys_setenv(const char *name, const char *value, int override) OPTIONAL_MC_FN;
EXTERN int sa_sys_getpid(void) OPTIONAL_MC_FN;
EXTERN FILE *sa_sys_popen(const char *command, const char *type) OPTIONAL_MC_FN;
EXTERN int sa_sys_pclose(FILE *s) OPTIONAL_MC_FN;

EXTERN char *sa_sys_fgets(char *str, int n, FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_fputs(const char *str, FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_fputc(int ch, FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_fseek(FILE *stream, long int offset, int whence) OPTIONAL_MC_FN;
EXTERN FILE *sa_sys_fopen(const char *s, const char *mode) OPTIONAL_MC_FN;
EXTERN int sa_sys_fclose(FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_fflush(FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_feof(FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_getc(FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_ungetc(int ch, FILE *s) OPTIONAL_MC_FN;
EXTERN size_t sa_sys_fread(void *ptr, size_t size, size_t nmemb, FILE *s) OPTIONAL_MC_FN;
EXTERN size_t sa_sys_fwrite(const void *ptr, size_t size, size_t nmemb,
                            FILE *s) OPTIONAL_MC_FN;
EXTERN int sa_sys_stat(const char *pathname, struct stat *statbuf) OPTIONAL_MC_FN;

EXTERN int sa_sys_unlink(const char *pathname) OPTIONAL_MC_FN;
EXTERN DIR *sa_sys_opendir(const char *name) OPTIONAL_MC_FN;
EXTERN struct dirent *sa_sys_readdir(DIR *dirp) OPTIONAL_MC_FN;
EXTERN int sa_sys_closedir(DIR *dirp) OPTIONAL_MC_FN;
EXTERN int sa_sys_isdir(const char* fp) OPTIONAL_MC_FN;
EXTERN int sa_sys_rmdir(const char *dir);
EXTERN void sa_sys_free(void *ptr) OPTIONAL_MC_FN;
EXTERN void *sa_sys_malloc(size_t size) OPTIONAL_MC_FN;
EXTERN void *sa_sys_realloc(void *old, size_t size) OPTIONAL_MC_FN;

EXTERN int sa_sys_thread_begin(sa_worker fn, size_t arg) OPTIONAL_MC_FN;
EXTERN void sa_sys_thread_end(void) OPTIONAL_MC_FN;

#endif
