/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2020 Tore Risch, SA
 *
 * Description: sa.storage C interface
 ****************************************************************************/

#ifndef _sa_storage_h_
#define _sa_storage_h_

#include <setjmp.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <limits.h>

#include "sa_handle.h"

/*** Utility functions ***/

EXTERN double a_frand(double lower, double upper);     /* Random real number */
EXTERN double a_round(double x);             /* Round float to nearest float */
EXTERN LONGINT a_roundi(double x);         /* Round float to nearest integer */
EXTERN int a_isupper(const char *str);          /* Is string str upper case? */
EXTERN void a_toupper(char *str);         /* Upper case string destructively */
EXTERN void a_tolower(char *str);         /* Lower case string destructively */
EXTERN char *a_toUnixFileName(char *file);   /* Convert \ to / destructively */
EXTERN void a_capitalize(char *str);      /* Capitalize string destructively */
EXTERN char *a_fullpath(const char *file);              /* Full path to file */
#define COMPARE(x,y) ((x)<(y)?-1:((x)>(y)?1:0))

/*** System Constants. Cannot be changed by user! ***/

#define PERMANENTREF 254    /* Reference counter value for permanent objects */
#define DEALLOCREF 255    /* Reference counter value for deallocated objects */
#define MAX_LENGTH 65535U                        /* Max size of 2 bytes area */

typedef ohandle a_size_t;                      /* Size of field inside image */
#define NULLH (ohandle)0                                   /* Illegal handle */

/*** Basic error numbers ***/

#define ARG_NOT_SYMBOL 21
#define ARG_NOT_LIST 3
#define ARG_NOT_NUMBER 10
#define ARG_NOT_STRING 11
#define ARG_NOT_ARRAY 12
#define ARRAY_BOUNDS 13
#define ZERO_DIVIDE 14
#define ARG_NOT_STREAM 30
#define ARG_NOT_TEXTSTREAM 27
#define ARG_NOT_ADJARRAY 31
#define NOT_SUPPORTED 38
#define ILLEGAL_ARGUMENT 4
#define SYSTEM_ERROR 39

/*** Basic physical object access ***/

EXTERN image_base_t *begin_image;         /* Start address of database image */
#define bytes2handle(X) (ohandle)((X)/sizeof(image_base_t))
EXTERN void a_expand_image(size_t size);            /* Expand database image */
typedef unsigned char objtype;                            /* Type identifier */
typedef unsigned char objrefcnt;                        /* Reference counter */
typedef struct objtags_rec{objtype ttag; objrefcnt rcnt;} objtags;
#define typetag(o) ((o)->tags.ttag)        /* type id of dereferenced object */

#ifdef SPLIT_IMAGE
#include "splitimage.h"
extern const image_base_t flash_image[SPLAYPOINT/sizeof(image_base_t)];
#define splay(X) ((X)<bytes2handle(SPLAYPOINT) ? flash_image : begin_image)
#else
extern a_size_t ram_position;
#define splay(X) begin_image
#endif

INLINE struct listcell *doid(ohandle x)      /* Generic object dereferencing */
{
  return (struct listcell *)(splay(x) + x);
}
#define dr(x,cell) ((struct cell *)doid(x))     /* Dereference object handle */
#define a_datatype(h) typetag(doid(h))      /* get type tag of object handle */
EXTERN short a_typenameid(const char *name);  /* get type tag for type named */
EXTERN const char *a_typename(int id);          /* Get type name of type tag */
EXTERN ohandle interror(int, ohandle);                           /* internal */
EXTERN int equal(ohandle,ohandle);                             /* Deep equal */
EXTERN int a_compare(ohandle,ohandle);/* less => -1, equal => 0, greater => 1*/

/*** These casting macros avoid X64 compiler warnings: ***/

#define handle_to_address(h) ((size_t)h)
#define void_to_handle(x) (ohandle)handle_to_address(x)
#define handle_to_void(h) (void *)handle_to_address(h)

/*** Binding environment access ***/

struct bindenv        /* A variable binding or a stack frame start indicator */
{
  ohandle var;                               /* Pointer to variable (symbol) */
  ohandle val;                       /* Pointer to value binding of variable */
  struct bindenv *env;     /* not NULL only in frame head. The dynamic link. */
};

typedef struct bindenv *bindtype;

//#include "sa_debug.h"

/* Raise and throw errors: */
EXTERN int a_raise_errormsg(const char *msg, ohandle obj);
EXTERN int a_raise_errorno(int no, ohandle obj);
EXTERN ohandle a_throw_error(void);
EXTERN ohandle a_throw_errormsg(bindtype env, const char *msg, ohandle obj);
EXTERN ohandle a_throw_errorno(bindtype env, int no, ohandle obj);
EXTERN void a_warning(const char *msg, ohandle obj);

#define OfType(h,tpe,env) if(a_datatype(h) != (tpe))                    \
    return a_throw_errorno(env,ILLEGAL_ARGUMENT,h);/* Test if h has type tpe */

/*** Lists ***/

#define LISTTYPE 0
#define listp(h) (a_datatype(h)==LISTTYPE)
EXTERN ohandle cons(ohandle x,ohandle y);         /* Classical Lisp function */

struct listcell
{
  objtags tags;
  ohandle head;
  ohandle tail;
};

#define fhd(h) (doid(h)->head)          /* fast (unsafe) head of linked list */
#define ftl(h) (doid(h)->tail)          /* fast (unsafe) tail of linked list */

INLINE ohandle hd(ohandle h)                     /* Safe head of linked list */
{
  return (listp(h)?fhd(h):interror(ARG_NOT_LIST,h));
}

INLINE ohandle tl(ohandle h)                     /* Safe tail of linked list */
{
  return (listp(h)?ftl(h):interror(ARG_NOT_LIST,h));
}

EXTERN int a_length(ohandle);                              /* Length of list */
EXTERN ohandle a_nth(ohandle l, int i);   /* Nth element in list, start w. 0 */
EXTERN ohandle a_list(ohandle,...);             /* Make list, given elements */
EXTERN ohandle nconc(ohandle,...);        /* Destructively concatenate lists */
EXTERN ohandle nreversefn(bindtype env, ohandle l);

/*** Symbols ***/

#define SYMBOLTYPE 1
#define symbolp(h) (a_datatype(h)==SYMBOLTYPE)
EXTERN ohandle new_symbol(const char *str);                      /* Internal */
#define mksymbol(s) new_symbol(s)           /* Make new symbol from string s */

struct symbolcell                                           /* aLisp symbols */
{
  objtags tags;
  unsigned short int flags;
  ohandle value;                                             /* global value */
  ohandle propl;                                            /* property list */
  ohandle pname;                                               /* print name */
  ohandle fndef;                                      /* function definition */
};

EXTERN ohandle t, starsymbol, quotesymbol;        /* Symbols T, *, and QUOTE */
EXTERN ohandle truesymbol, falsesymbol, tuplesymbol;

#define globval(h)dr(h,symbolcell)->value     /* Global value of Lisp symbol */
EXTERN ohandle locvalfn(bindtype env,
                        ohandle var);          /* Local value of Lisp symbol */
#define getpname(h)getstring(dr(h,symbolcell)->pname)/* Print name of symbol */

/*** Numbers ***/

#define integerp(h) (a_datatype(h)==INTEGERTYPE)
EXTERN ohandle new_integer(LONGINT i);                           /* Internal */
#define mkinteger(i) new_integer(i)           /* Make new integer from C int */

struct integercell
{
  objtags tags;
  HEADFILLER;
  char data[sizeof(LONGINT)];
};

INLINE LONGINT getinteger(ohandle h)
{
  LONGINT res;

  memcpy(&res, dr(h,integercell)->data, sizeof(res));
  return res;
}

#define getinteger32(h)(int)getinteger(h)

#define realp(h) (a_datatype(h)==REALTYPE)
EXTERN ohandle new_real(int dummy,double x);                     /* Internal */
#define mkreal(r) new_real(0,r)
/* Make new real from C double */

struct realcell
{
  objtags tags;
  char real[sizeof(double)];            /* 'double' alignment not guaranteed */
};

EXTERN double coerce_real(bindtype env,ohandle h);
/* Get C double from number object */

EXTERN double getreal(ohandle h);      /* Get contents of REAL object handle */
EXTERN float getfloat(ohandle h);      /* Get contents of REAL object handle */

/*** Numeric dereferencing macros ***/

#define numberp(h) (NUMERIC(a_datatype(h)))

#define IntoInteger(h,into,env) if(integerp(h)) into = getinteger(h);   \
  else if(realp(h)) into = (LONGINT)getreal(h);                         \
  else return a_throw_errorno(env,ARG_NOT_NUMBER,h) /* Dereference a LONGINT */

#define IntoInteger32(h,into,env) if(integerp(h)) into = getinteger32(h); \
  else if(realp(h)) into = (int)getreal(h);                             \
  else return a_throw_errorno(env,ARG_NOT_NUMBER,h)    /* Dereference an int */

#define IntoDouble(h,into,env) if(realp(h)) into = getreal(h);          \
  else if(integerp(h)) into = getinteger(h)+0.0;                        \
  else return a_throw_errorno(env,ARG_NOT_NUMBER,h)  /* Dereference a double */

#define IntoFloat(h,into,env) if(realp(h)) into = getfloat(h);          \
  else if(integerp(h)) into = getinteger(h)+(float)0.0;                 \
  else return a_throw_errorno(env,ARG_NOT_NUMBER,h)    /* Dereference a real */

/*** Strings ***/

#define stringp(h) (a_datatype(h) == STRINGTYPE)
EXTERN ohandle new_string(size_t len,const char *str);          /* Internal */
#define mkstring(s) new_string(strlen(s)+1,s)
/* Make new STRING object from C char* s */

struct stringcell                                      /* Fix length strings */
{
  objtags tags;
  unsigned short int shortlen;/* Length in bytes including terminating null
                                 character                                   */
  union {char string[1];
    a_size_t longlen;} cont;               /* The string is padded here */
};

#define dgetstring(s) ((s)->shortlen == MAX_LENGTH ?                    \
		       (s)->cont.string+sizeof((s)->cont.longlen):      \
                       (s)->cont.string)
INLINE char *getstring(ohandle h)    /* Get contents of STRING object handle */
{
  struct stringcell *dh = dr(h,stringcell);
  return dgetstring(dh);
}
#define dstringlen(s) ((s)->shortlen == MAX_LENGTH ?            \
		       (s)->cont.longlen : (s)->shortlen)
/* length of dereferenced string */
INLINE a_size_t stringlen(ohandle h)   /* Get length of string object handle */
{
  struct stringcell *dh = dr(h,stringcell);
  return dstringlen(dh);
}

EXTERN char *a_to_string(ohandle h);
/* Convert h into C new malloced string buffer */

#define IntoStringRef(h,into,env) if(stringp(h)) into = getstring(h);   \
  else if(symbolp(h)) into = getpname(h);                               \
  else return a_throw_errorno(env,ARG_NOT_STRING,h)
/* Dereference string or symbol.
   Returns pointer into image   */

#define StackString(s,p)(p = (char *)alloca(strlen(s)+1), strcpy(p,s))
#define IntoString(h,into,env)                          \
  if(stringp(h)) StackString(getstring(h),into);        \
  else if(symbolp(h)) StackString(getpname(h),into);    \
  else return a_throw_errorno(env,ARG_NOT_STRING,h)
/* Safe dereferencing of string if string is short.
   Copy string to stack */

/*** Arrays ***/

#define arrayp(h) (a_datatype(h) == ARRAYTYPE)
EXTERN ohandle new_array(int len,ohandle init);

struct arraycell                                /* Fixed size small 1D array */
{
  objtags tags;
  unsigned short int size;
  ohandle cont[1];              /* Variable length object with array elements
                                   allocated starting here                   */
};

struct adjarraycell                            /* Growable or large 1D array */
{                                             /* Same type tag as arraycell! */
  objtags tags;
  unsigned short int small_size;                        /* Always MAX_LENGTH */
  int size;
  int top;                     /* position of last segment in segments array */
  ohandle init;                                             /* initial value */
  ohandle segments;                    /* pointer to array of array segments */
};
#define is_adjustable_array(x)(((struct arraycell *)x)->size == MAX_LENGTH)

EXTERN int a_arraysize(ohandle arr);                    /* Get size of ARRAY */
EXTERN ohandle copy_arrayfn(bindtype env, ohandle a);          /* Copy array */
EXTERN ohandle copy_array(bindtype env, ohandle a, int newsize,
                          int adjustable);   /* Copy to bigger/smaller array */
EXTERN ohandle new_adjarray(int size,ohandle init);  /* New adjustable array */
EXTERN int adjust_array(ohandle a, int newsize);
/* Increase size of array in-place. Returns TRUE if adjustment successful */
EXTERN ohandle listtoarrayfn(bindtype, ohandle);    /* Convert list to array */
EXTERN ohandle arraytolistfn(bindtype,ohandle);     /* Convert array to list */
EXTERN ohandle a_elt(ohandle,int);                   /* Access array element */
EXTERN ohandle a_seta(ohandle,int,ohandle);             /* Set array element */
EXTERN ohandle a_vector(ohandle,...);              /* Make array of elements */
/* Add x to end of array by adjusting it. Returns adjusted or new array: */
EXTERN ohandle push_vectorfn(bindtype env, ohandle a, ohandle x);
EXTERN ohandle *a_kvpsort(int size, ohandle *kvp_vector);
EXTERN ohandle *a_sortvector(int size, ohandle *vector);

/*** Numeric binary arrays ***/

#define NUMARRAYTYPE 16

/*** Surrogate types ***/

EXTERN ohandle new_oid(int idno);                                 /* New OID */

/*** Hash tables ***/

#define HASHTYPE 10
#define HASHBUCKETTYPE 11
EXTERN a_size_t compute_hash_key(ohandle key);
/* Compute hash key of any object */
typedef int (*maphash_function)(bindtype,ohandle,ohandle,ohandle,void *);
EXTERN ohandle new_hashtable(unsigned int equalflag);
/* Allocate new hash table. equalflg=TRUE => test with EQUAL otherwise EQ */
EXTERN int put_hashtable(ohandle ht, ohandle key, ohandle val);
/* Insert key with associated value val in hash table ht */
EXTERN ohandle get_hashtable(ohandle ht, ohandle key);
/* Get associated value of key in hash table ht */
EXTERN ohandle rem_hashtable(ohandle ht, ohandle key);
/* Remove key and its associated value from hash table ht */
EXTERN ohandle remunlink_hashtable(ohandle ht, ohandle key);
/* Unlink associated value and remove key from ht */
EXTERN void map_hashtable(bindtype env, ohandle indhdr, ohandle ht,
                          maphash_function f, void *x);
/* Iterate over keys in hash table ht applying C function fn(key,val,xa) */
EXTERN void clear_hashtable(ohandle ht);
/* Remove all keys/values from hash table ht */

/*** Records ***/

#define recordp(h) (a_datatype(h)==RECORDTYPE)
EXTERN ohandle a_record(const char* k, ...);
EXTERN ohandle new_record(void);
EXTERN ohandle record_put(ohandle r, char *key, ohandle val);
EXTERN ohandle record_get(ohandle r, char *key);

/*** Foreign functions ***/

#define EXTFNTYPE 4

/*** Lisp closures ***/

#define CLOSURETYPE 5

/*** Indexes ***/

#define INDEXTYPE 15

/*** Events ***/

EXTERN int eventtagtype;               /* Type tag for storage type eventtag */
struct eventtag                                        /* Tag for log events */
{
  objtags tags;
  ohandle name;
  ohandle undofn;
  ohandle redofn;
  ohandle eventfns;
};
typedef int (*event_handler)(bindtype,ohandle,ohandle,ohandle,ohandle,ohandle);
/* Hook for raising events before being added to log */
/* FALSE result => event not added to log */
EXTERN event_handler raise_event;        /* Called before event added to log */

/*** Byte stream headers ***/

struct streamheader
/* This struct must always follow tags in any kind of byte stream template */
{
  short int bytes;            /* Total size of object in bytes, incl. header */
  unsigned char autoflush:1;           /* Flush after each item and new line */
  unsigned char systime:1;                       /* Maintain current systime */
  unsigned char newline:1;                   /* True when \n is just printed */
  unsigned char textual:1;                             /* Non-binary if true */
  unsigned char displayed:1;                 /* Displayed on console id true */
  signed char architecture;                           /* Target architecture */
  int line_num;                                       /* Current line number */
  ohandle logstream;                   /* Stream to copy input to if non-NIL */
  ohandle origin;                    /* ID of sender of data if known or nil */
  ohandle destination;             /* ID of receiver of data if known or nil */
  ohandle propl;                                   /* Dynamic  property list */
};

EXTERN void init_streamheader(struct streamheader *sh);/* Initializing header*/
EXTERN void free_streamheader(struct streamheader *sh);  /* Finalizing header*/
EXTERN ohandle stream_getfn(bindtype env, ohandle s, ohandle prop);
EXTERN ohandle stream_putfn(bindtype env, ohandle s, ohandle prop, ohandle val);
EXTERN ohandle stream_proplfn(bindtype env, ohandle s);
#define a_streamp(h) (typefns[a_datatype(h)].stream_index != -1)
/* TRUE is h is a stream */

/*** Character streams ***/

#define STREAMTYPE 8
EXTERN ohandle new_stream(FILE *fp, int tailed);     /* Open new file stream */
#define mkstream(x) new_stream(x,FALSE)

struct streamcell                                         /* OS file streams */
{
  objtags tags;
  struct streamheader header;
  int opened;                                    /* TRUE while stream opened */
  int tailed;                                      /* TRUE if file is tailed */
  FILE *fp;                                               /* OS file pointer */
};

#define getstream(h) dr(h, streamcell)->fp
#define line_num_of(h) dr(h,streamcell)->header.line_num
EXTERN ohandle a_fopen(const char *filename,
                       const char *mode);                /* Open file stream */
EXTERN int a_fclose(ohandle stream);                         /* Close stream */
EXTERN ohandle stdoutstream,                       /* standard output stream */
  stdinstream;                         /* standard input stream */
EXTERN ohandle logstream;              /* Text stream for logging OSQL input */
EXTERN ohandle eofsymbol;                /* Object returned when EOF reached */
EXTERN size_t a_puts(const char *str,ohandle stream);        /* Write string */
EXTERN int a_putc(int c, ohandle stream);               /* Write a character */
EXTERN size_t a_puti(LONGINT i, ohandle stream);         /* Write an integer */
EXTERN size_t a_putr(double r, ohandle stream);              /* Write a real */
EXTERN size_t a_writebytes(ohandle stream, const void *buff, size_t len);
/* Write buffer */
EXTERN int a_fflush(ohandle stream);                  /* Flush stream buffer */
EXTERN void a_printf(ohandle s, const char *fmt,...);     /* Formatted print */
EXTERN void a_message(const char *msg, ...);    /* Formatted console message */
EXTERN int a_getc(ohandle stream);                     /* Read one character */
EXTERN int a_ungetc(int c, ohandle stream);          /* Unread one character */
EXTERN size_t a_readbytes(ohandle stream, void *buff, size_t len);
/* Read buffer */
EXTERN int a_feof(ohandle stream);                   /* Test for end-of-file */

EXTERN ohandle a_read(ohandle stream);                  /* Read S-expression */
EXTERN ohandle a_read_from_buffer(const void *buffer, size_t len);
/* Read S-expression from buffer */
EXTERN ohandle a_read_from_string(const char *str);
/* Read S-expression from C string */
EXTERN ohandle a_print(ohandle s);  /* Print S-expression CR on stdoutstream */
EXTERN ohandle a_printobj(ohandle s, ohandle stream);
/* Print S-expression followed by CR on stream */
EXTERN ohandle a_prin1(ohandle s, ohandle stream, int flag);
/* Print S-expression. flag=TRUE => PRINC */
EXTERN void a_printstring(char *str,ohandle stream,int princflg);
/* print C string as prin1 */
EXTERN ohandle a_terpri(ohandle stream);                         /* Print CR */

/*** Text streams ***/

#define TEXTSTREAMTYPE 9
EXTERN ohandle new_textstream(size_t len, int textual, int diaplayed);
EXTERN ohandle map_textstream(void *mem, size_t len);

struct textstreamcell                           /* stream over string buffer */
{
  objtags tags;
  struct streamheader header;
  size_t size;                                         /* The size of buffer */
  size_t pos;                           /* Current cursor position in buffer */
  size_t end;                                                /* EOF position */
  ohandle buffer;                         /* Pointer to buffer as binarycell */
};
EXTERN ohandle a_textstreamread(ohandle stream, size_t from);
/* Read S-expression */

/*** Memory areas inside image ***/

#define BINARYTYPE 13

struct binarycell          /* 4-bytes aligned field of bytes, max length 64K */
{
  objtags tags;
  unsigned short int size;
  unsigned char cont[1];                             /* Contents padded here */
};

EXTERN ohandle alloc_binary(a_size_t size);
EXTERN ohandle new_binary(const void *data, a_size_t size);

/*** Memory areas outside image ***/

#define MEMORYTYPE 18

struct memorycell
{
  objtags tags;
  unsigned short int mapped;
  void(*release_callback)(void *memory, size_t mapped_context);
  size_t mapped_context;
  size_t size;
  void *data;
};

EXTERN ohandle alloc_memory(size_t size);
EXTERN ohandle realloc_memory(ohandle m, size_t size);
EXTERN ohandle map_memory(const void *mem, size_t size, size_t context,
                          void(*release_callback)(void *memory,
                                                  size_t mapped_context));
EXTERN ohandle make_memory(const void *mem, size_t size);

/*** Time ***/

EXTERN char *a_localtime(void);
EXTERN ohandle a_maketimeval(sa_time_t tv);
EXTERN sa_time_t a_gettime(ohandle o);
EXTERN void a_sleep(double t);                            /* Sleep t seconds */

#define CODETYPE 19

EXTERN unsigned short int TIMEVALTYPE;

/*** Variable binding stack for aLisp and SLOG ***/

EXTERN int a_stacksize;        /* Size of stack. Can be set before init_amos */
EXTERN struct bindenv *varstack;     /* Points to the current variable stack */
EXTERN int varstacksize;          /* The overflow point of the current stack */
EXTERN bindtype varstacktop;                            /* Current stack top */
EXTERN bindtype topframe(void);           /* The current binding environment */

/*** Interrupt handling ***/

EXTERN void a_interrupt(void);                         /* Raise an interrupt */
#define CheckInterrupt  if(InterruptHasOccurred) DoInterrupt()
/* Check if an interrupt has occurred */
EXTERN int InterruptHasOccurred, InterruptEnabled, YieldInterrupt;
EXTERN void DoInterrupt(void);         /* Execute a pending interrupt action */
EXTERN void (*a_interrupt_handler)(int);

/*** C function pointers ***/

EXTERN FILE *(*a_fopener)(const char *filename,
                          const char *mode);       /* Customized file opener */

typedef ohandle(*error_handler)(bindtype,int,const char*,ohandle);
EXTERN error_handler a_error_handler;         /* Called after error detected */

typedef void(*expand_handler)(size_t,size_t);
EXTERN expand_handler expand_demon;     /* Called when image watermark grows */

EXTERN void (*a_exit_handler)(int);          /* Called when quit is executed */

EXTERN ohandle(*a_object_printer)(ohandle);        /* Generic object printer */

/*** Table of physical types ***/

typedef void (*print_function) (ohandle,ohandle,int);        /* print object */
typedef void (*dealloc_function) (ohandle);                /* dealloc object */
typedef a_size_t(*hash_function)(ohandle);                 /* hash on object */
typedef int (*equal_function)(ohandle,ohandle);                /* equal test */
typedef ohandle (*reader_function)(bindtype,ohandle tag,
                                   ohandle descr, ohandle stream);
/* type reader function */
#ifdef SPLIT_IMAGE
#define type_reader_function(X,Y)
#else
EXTERN void type_reader_function(const char *tpe, reader_function fn);
#endif
struct typefnsslot                   /* Type functions, stored outside image */
{
  dealloc_function deallocfn;             /* Called by GBC to deallocate obj */
  print_function printfn;                           /* Prints object on file */
  equal_function equalfn;                           /* returns TRUE or FALSE */
  equal_function comparefn;                           /* Returns -1, 0, or 1 */
  hash_function hashfn;                              /* Returns hash integer */
  short stream_index;                                /* != -1 if stream type */
  a_size_t size;                                     /* Static size in bytes */
  size_t(*sizefn)(ohandle, int);                    /* Dynamic size in bytes */
  void (*restorer)(ohandle, FP);   /* Optional restorer of registered object */
  void (*saver)(ohandle, FP);                    /* Optional saver of object */
  const char *name;                                     /* name of data type */
};

struct typedataslot                            /* Type data, stored in image */
{
  int alloccnt;                                       /* allocation counter */
  int dealloccnt;                                   /* deallocation counter */
  int total;                           /* total number of objects allocated */
  int max;                               /* Max number of objects allocated */
};

EXTERN struct typefnsslot *typefns;

EXTERN short int a_definetype(const char *name,dealloc_function,print_function);
/* Define new physical object type with name, destructor and print function */


/*** Stream implementations ***/

struct stream_implementation
{
  int(*getc)(ohandle);
  int(*ungetc)(int,ohandle);
  int(*putc)(int,ohandle);
  int(*fclose)(ohandle);
  int(*feof)(ohandle);
  int(*fflush)(ohandle);
  size_t(*writebytes)(ohandle,void *,size_t);           /* NULL => putc used */
  size_t(*readbytes)(ohandle,void *,size_t);            /* NULL => getc used */
};

EXTERN struct stream_implementation *stream_implementations;
#define get_stream_implementation(ds) stream_implementations[typefns[typetag(ds)].stream_index]

EXTERN int a_define_stream_implementation(int tag,           /* Storage type */
                                          int(*getc)(ohandle),
                                          int(*ungetc)(int,ohandle),
                                          int(*feof)(ohandle),
                                          size_t(*writebytes)(ohandle,void*,size_t),
                                          size_t(*readbytes)(ohandle,void*,size_t),
                                          int(*putc)(int,ohandle),
                                          int(*fflush)(ohandle),
                                          int(*fclose)(ohandle));

/*** GBC interface macros ***/

#define rcnt(x) ((x)->tags.rcnt)  // reference counter of dereferenced object
#define refcntp(x)(rcnt(x)!=PERMANENTREF)
/* TRUE if garbage collected dereferenced object */
#define incref(x) (void)(refcntp(x) && rcnt(x)++)// Increase reference counter
#define decref(x) (void)(refcntp(x) && rcnt(x)--)// Decrease reference counter
EXTERN void release(ohandle o);                     /* Free o if refcnt == 0 */
INLINE void released(ohandle h)
{                             /* Decrease reference counter of object handle
                                 and free if counter becomes 0            */
  register struct listcell *_dh = doid(h);
  if(refcntp(_dh) && rcnt(_dh)-- == 1) release(h);
}

/* Initialize handle location: */
#define a_let(var,val)                                                  \
  {ohandle __initval=(val); incref(doid(__initval)); (var)=__initval;}
/* Reassign handle location: */
#define a_setf(dest,val)                                                \
  {ohandle __newval=(val); incref(doid(__newval)); ohandle __oldval= dest; \
    dest=__newval; released(__oldval);}

/* Decrease reference counter without deallocating object when returning
   handle from function: */
#define a_return(val) {decref(doid(val)); return(val);}

/* Release object location: */
#define a_free(dest) {released(dest);dest=nil;}

/*** Storage allocation and deallocation ***/

EXTERN ohandle new_object(int size, int type);
struct objectcell                /* Template for word aligned storage object */
{
  objtags tags;                                              /* System tags */
  short int bytes;           /* Total size of object in bytes, incl. header */
  char cont[sizeof(int)] ;                           /* Content padded here */
};

EXTERN void dealloc_object(ohandle h);
/* Deallocate word aligned storage object */

EXTERN ohandle new_aligned_object(a_size_t size, int type);
/* Allocate new double word aligned storage object. */
/* Allocate new word aligned storage object with size 'size' and
   type tag 'type' */
struct aligned_objectcell              /* Double word aligned storage object */
{
  objtags tags;                                              /* System tags */
  short int filler;                                   /* Not used by system */
  a_size_t bytes;            /* Total size of object in bytes, incl. header */
  int32_t cont[1];             /* Content padded here. Double word aligned. */
};

EXTERN void dealloc_aligned_object(ohandle h);
/* Deallocate double word aligned storage object */

EXTERN void a_initialize_object(ohandle o);       /* Initialize and register */

/*** Error handling ***/

EXTERN jmp_buf *resetlabelp;                          /* Current reset point */
EXTERN ohandle the_evaluated_form;                    /* Last evaluated form */

EXTERN int a_register_error(const char *msg);/* Register a new error message */
EXTERN int a_errornumber(const char *msg); /* The number of an error message */
EXTERN const char *a_errormessage(int no);    /* Get the error message of no */
EXTERN int a_lasterrno;                /* The currently highest error number */
EXTERN void fatalexit(int);                        /* Error that causes exit */
EXTERN int a_lock(void);                  /* Lock sa.engine for other threads*/
EXTERN int a_lock_count(void);  /* No of engine locks held by current thread */
EXTERN void a_unlock(void);                              /* Unlock sa.engine */
EXTERN void free_oid(ohandle h);                       /* Thread safe a_free */
#define a_assign(lhs,rhs) sa_assign(&(lhs),rhs)        /* Thread safe a_setf */
EXTERN ohandle resetfn(bindtype);                      /* Throw an exception */
EXTERN void clearstack(bindtype);                                /* Internal */

/*** Unwind protect ***/
EXTERN bindtype topenv;                              /* Internal. Do not use */

#define unwind_protect_begin0(label) volatile bindtype thisenv = topenv; \
  jmp_buf *volatile oldresetlabelp=resetlabelp; int unwind_reset = FALSE; \
  jmp_buf newresetlabel; resetlabelp=&newresetlabel;                    \
  if(setjmp(newresetlabel)) {clearstack(thisenv);                       \
    unwind_reset = TRUE; goto label;}

#define unwind_protect_catch0(label)            \
  label: resetlabelp = (jmp_buf*)oldresetlabelp
/* Internal macros */

#define unwind_protect_begin unwind_protect_begin0(unwind_protect_catchpoint)
/* Start unwind-protect block */
#define unwind_protect_catch unwind_protect_catch0(unwind_protect_catchpoint)
/* Start the unconditional catch section */
#define unwind_protect_end if(unwind_reset) resetfn(thisenv);
/* End unwind-protect block */
#define unwind_protect_cancel unwind_reset=FALSE; unwind_protect_end

/*** Configuration variables ***/

EXTERN char *a_default_image;       /* Assign name of default database image */
EXTERN char *a_startupdir;                       /* Assign startup directory */
EXTERN float a_image_expansion_factor;
/* Factor to automatically expand image with when full (dflt. 1.25).
   Image is pinned and unexpandible when factor is 1 */
EXTERN int a_always_move_image;
/* TRUE => image always moves at expansions (dflt. FALSE) */

#endif

