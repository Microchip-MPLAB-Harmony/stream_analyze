/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2020 Tore Risch, SA
 *
 * Description: Interface to threads
 ****************************************************************************/

#ifndef _sa_threads_h
#define _sa_threads_h

#include "sa_client.h"

typedef void (*sa_worker)(size_t arg);
EXTERN int sa_thread_begin(sa_worker fn, size_t arg);
EXTERN void sa_thread_end(void);
EXTERN int sa_thread_initialize(void);
EXTERN int sa_thread_finalize(void);
EXTERN int sa_thread_enabled(void);
EXTERN int sa_thread_id(void);

#endif
