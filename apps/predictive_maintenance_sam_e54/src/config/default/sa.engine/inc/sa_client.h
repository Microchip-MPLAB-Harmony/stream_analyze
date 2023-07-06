/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2020 Tore Risch, SA
 *
 * Description: Thread safe sa.engine C client API 2.0
 ****************************************************************************/

#ifndef _sa_client_h
#define _sa_client_h

#include "sa_handle.h"

typedef ohandle sa_connection;
typedef ohandle sa_stream;
typedef ohandle sa_tuple;
typedef ohandle sa_record;
typedef ohandle sa_binary;
typedef int (*sa_callback)(sa_tuple res, void *xa);

/*** Initializing ***/
EXTERN int sa_engine_init(int argc, char **argv);            /* Start engine */
EXTERN int sa_connect(sa_connection *c, const char *peer);
EXTERN int sa_disconnect(sa_connection c);
EXTERN int sa_engine_console(void);
EXTERN sa_connection local_connection;

/*** Generic object handles ***/
EXTERN int sa_typetag(ohandle h);              /* Storage type tag of object */
EXTERN const char *sa_storagetype(int typetag);      /* Name of storage type */
EXTERN size_t sa_size(ohandle h);                          /* Size of object */
EXTERN int sa_print(ohandle h);                     /* Print any OSQL object */
EXTERN void sa_assign(ohandle *lhs, ohandle rhs);   /* Set lhs handle to rhs */
EXTERN int sa_stringify(ohandle *str, ohandle obj); /* Put string representation
                                                       of obj into str */
/*** Tuples ***/
EXTERN int sa_maketuple(sa_tuple *tpl, int sz);
EXTERN int sa_getelem(ohandle *res, sa_tuple tpl, int pos);
EXTERN int sa_setelem(sa_tuple tpl, int pos, ohandle val);
EXTERN int sa_elemsize(size_t *l, sa_tuple tpl, int pos);

/*** Booleans ***/
EXTERN int sa_isbool(ohandle h);
EXTERN int sa_getbool(int *res, ohandle h);
EXTERN int sa_makebool(ohandle *res, int flag);
EXTERN int sa_getboolelem(int *flag, sa_tuple tpl, int pos);
EXTERN int sa_setboolelem(sa_tuple tpl, int pos, int flag);

/*** Numbers ***/
EXTERN int sa_makedouble(ohandle *res, double x);
EXTERN int sa_getdouble(double *res, ohandle h);
EXTERN int sa_makeinteger(ohandle *res, LONGINT i);
EXTERN int sa_getinteger(LONGINT *res, ohandle h);
EXTERN int sa_getint32(int *res, ohandle h);
EXTERN int sa_setintelem(sa_tuple tpl, int pos, LONGINT val);
EXTERN int sa_getintelem(LONGINT *res, sa_tuple tpl, int pos);
EXTERN int sa_getint32elem(int *res, sa_tuple tpl, int pos);
EXTERN int sa_setdoubleelem(sa_tuple tpl, int pos, double val);
EXTERN int sa_getdoubleelem(double *res, sa_tuple tpl, int pos);

/*** Strings ***/
EXTERN int sa_makestring(ohandle *res, const char *str);
EXTERN int sa_getstring(char *buff, size_t buffsize, ohandle str);
EXTERN int sa_setstringelem(sa_tuple tpl, int pos, const char *str);
EXTERN int sa_getstringelem(char *buff, size_t buffsize, sa_tuple tpl, int pos);

/*** Time ***/
#define sa_time_second(tp) tp*1.0E-6
#define sa_time_compare(tpx,tpy) COMPARE(tpx,tpy)
#define sa_time_shift(tp, usec) (LONGINT)(tp+usec)
#define sa_time_span(tpx, tpy) (LONGINT)(tpy-tpx)
EXTERN double sa_rnow(void);   // Seconds since EPOCH 1970-01-01
EXTERN sa_time_t sa_now(void); // Microseconds since EPOCH 1970-01-01
EXTERN int sa_sleep(double sec);
EXTERN int sa_maketimeval(ohandle *res, sa_time_t tp);
EXTERN int sa_gettime(sa_time_t *res, ohandle o);
EXTERN int sa_gettimeelem(sa_time_t *tp, sa_tuple tpl, int pos);
EXTERN int sa_settimeelem(sa_tuple tpl, int pos, const sa_time_t tp);

/*** Binary areas ***/
EXTERN int sa_makebinary(sa_binary *res, const void *buff, size_t buffsize);
EXTERN int sa_getbinary(void *buff, size_t buffsize, size_t *len, sa_binary b);
EXTERN int sa_getbinaryelem(void *buffer, size_t buffsize, size_t *len,
                            sa_tuple tpl, int pos);
EXTERN int sa_setbinaryelem(sa_tuple tpl, int pos, const void *buffer,
                            size_t buffsize);
EXTERN int sa_addbinaryelem(sa_tuple tpl, int pos, const void *buffer,
                            size_t buffsize);

/* Memory areas */
EXTERN int sa_makememory(ohandle *b, const void *buffer, size_t buffsize);
EXTERN int sa_map_memory(ohandle *b, const void *buffer, size_t buffsize,
                         size_t context,
                         void(*release_callback)(void *memory, size_t size));

/*** Records ***/
EXTERN int sa_makerecord(sa_record *res);
EXTERN int sa_getrecord(ohandle *val, sa_record r, const char *key);
EXTERN int sa_putrecord(sa_record r, const char *key, ohandle val);
EXTERN int sa_record_attributes(ohandle *val, sa_record r);
EXTERN int sa_parse_json(sa_record *o, const char *json_string);
/* Parse JSON to sa.engine object o */
EXTERN int sa_json_stringify(ohandle *str, ohandle h);
/* Put JSON string representation of o into str */

/*** Calling functions and executing queries ***/
EXTERN int sa_query(sa_stream *s, sa_connection c, const char *q);
EXTERN int sa_call(sa_stream *s, sa_connection c, const char *fn,
                   sa_tuple args);
EXTERN int sa_callfno(sa_stream *s, sa_connection c, ohandle fno,
                      sa_tuple args);
EXTERN int sa_map(sa_stream s, sa_callback cb, void *xa);
EXTERN int sa_run(sa_tuple *res, sa_stream s);
EXTERN int sa_runquery(sa_tuple *res, sa_connection c, const char *q);
EXTERN int sa_command(const char *stmt);
EXTERN int sa_runcall(sa_tuple *res, sa_connection c, const char *fn,
                      sa_tuple argl);
EXTERN int sa_terminate(sa_stream s);

/*** Release handle references ***/
EXTERN void sa_free(ohandle *h,...);

/*** Errors ***/
// Thread safe functions:
EXTERN int sa_errno(void);
EXTERN int sa_set_errno(int);
EXTERN const char *sa_errstr(void);
EXTERN const char *sa_strerror(int no);
EXTERN ohandle sa_errobject(void);
EXTERN int sa_raise_errormsg(const char *msg, ohandle obj);
EXTERN int sa_raise_errorno(int no, ohandle obj);
EXTERN int sa_print_error(void);
// Cannot be called concurrently:
EXTERN int sa_register_error(const char *msg); // Only in initializations

/*** Meta-data access ***/
EXTERN int sa_getfunction(ohandle *res, sa_connection c, const char *name);
EXTERN int sa_gettype(ohandle *res, sa_connection c, const char *name);
EXTERN int sa_typeof(ohandle *res, sa_connection c, ohandle h);
EXTERN int sa_getobjectno(ohandle *res, sa_connection c, int oidno);
EXTERN int sa_oidno(int *res, ohandle h);

/*** Database updates ***/
EXTERN int sa_createobject(ohandle *res, sa_connection c, ohandle tpo);
EXTERN int sa_deleteobject(sa_connection c, ohandle h);
EXTERN int sa_setfunction(sa_connection c, ohandle fno, sa_tuple argl,
                          sa_tuple resl);
EXTERN int sa_addfunction(sa_connection c, ohandle fno, sa_tuple argl,
                          sa_tuple resl);
EXTERN int sa_remfunction(sa_connection c, ohandle fno, sa_tuple argl,
                          sa_tuple resl);

/*** Utility functions ***/
EXTERN const char *sa_os(void); // Windows, Linux, OSX, FreeRTOS, etc.
EXTERN const char *sa_architecture(void); // Intel, ARMM, ARMA, etc.
INLINE size_t sa_wordlen(void){ return sizeof(size_t)*8;}
#endif
