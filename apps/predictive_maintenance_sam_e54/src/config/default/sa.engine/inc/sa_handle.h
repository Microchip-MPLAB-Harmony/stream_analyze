/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2020 Tore Risch, Stream Analyze Sweden AB
 *
 * Description: Representation of object handles
 *
 ****************************************************************************/

#ifndef sa_handle_h_
#define sa_handle_h_

#include <inttypes.h>

#include "environ.h"

#if defined(MICROCORE)
#define MAX_IMAGE_SIZE ((size_t)0x40000u)
typedef unsigned short int ohandle;
#define SYMBOL_SIZE 12

#elif defined(ESP)
#define MAX_IMAGE_SIZE 3800000
typedef unsigned int ohandle;
#define SYMBOL_SIZE 20

#else
#define MAX_IMAGE_SIZE ((size_t)0x3ffffffffull)
typedef unsigned int ohandle;
#define SYMBOL_SIZE 20
#endif

#define HEADFILLER short int filler

struct objectproto
{
  unsigned char prefix[4];
  ohandle hd;
  ohandle tl;
};

typedef LONGINT sa_time_t; // usec relative to EPOCH 1970-01-01
#define MILLIONLL 1000000LL

#define OBJECTSIZE sizeof(struct objectproto)

typedef uint32_t image_base_t;
#define nil (ohandle)(SYMBOL_SIZE/sizeof(image_base_t))

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define INTEGERTYPE 2
#define REALTYPE 3
#define NUMERIC(tag) ((tag==INTEGERTYPE) | (tag==REALTYPE))
#define STRINGTYPE 6
#define ARRAYTYPE 7
#define SURROGATETYPE 12
#define COMPLEXTYPE 14
#define RECORDTYPE 17

#endif
