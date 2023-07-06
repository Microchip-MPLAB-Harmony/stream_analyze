/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2020 Tore Risch, SA
 *
 * Description: Lisp <-> C interface
 ****************************************************************************/

#ifndef _sa_lisp_h_
#define _sa_lisp_h_

#include "sa_storage.h"
#include "sa_client.h"

/*** Foreign functions ***/

typedef ohandle (*Lispfunction) ();

struct extfncell /* C function called from aLisp, type EXTFN */
{
  objtags tags;
  short int evalargs; /* Number of arguments, NO_EVAL or NO_SPREAD */
  ohandle name;       /* Name of function */
  ohandle next;       /* next SUBR */
  Lispfunction fnaddr;/* Address of foreign Lisp function impelementation */
};
#define NOEVAL 0
#define NO_SPREAD -1
#define NO_EVAL -2

/*** aLisp closures ***/

struct closurecell
{
  objtags tags;
  short int hasstatlink;
  ohandle function, nxt, bindings, statlink;
  struct bindenv *varstack, *stkptr;
};

/*** Macros to pick up actual arguement when variable arity***/

#define nthargval(xenv,xpos) xenv[xpos].val
/* Value of n:th argument of SUBR */
#define arg_start(arg,env) (arg = env + 1)
/* initialize pointer to current argument */
#define arg_varp(arg) (arg->env==NULL)
/* TRUE if current arg is not last argument */
#define arg_next(arg) ((void)++arg)
/* increment arg list pointer */
#define arg_val(arg) arg->val
/* Value of current argument */
#define arg_nextval(arg) (arg++)->val
/* increment arg list pointer and get next value */
#define arg_var(arg) arg->var
/* The arity of a frame: */
EXTERN int envarity(bindtype env);

/*** Signatures of aLisp functions implemented in C ***/

typedef ohandle (*Lispfunction0) (bindtype);
typedef ohandle (*Lispfunction1) (bindtype,ohandle);
typedef ohandle (*Lispfunction2) (bindtype,ohandle,ohandle);
typedef ohandle (*Lispfunction3) (bindtype,ohandle,ohandle,ohandle);
typedef ohandle (*Lispfunction4) (bindtype,ohandle,ohandle,ohandle,ohandle);
typedef ohandle (*Lispfunction5) (bindtype,ohandle,ohandle,ohandle,ohandle,
                                  ohandle);
typedef ohandle (*Lispfunction6) (bindtype,ohandle,ohandle,ohandle,ohandle,
                                  ohandle,ohandle);
typedef ohandle (*Lispfunctionn) (bindtype,bindtype);

/*** Defining aLisp functions in C ***/

#ifdef SPLIT_IMAGE
#define  extfunction0(x, y)
#define  extfunction1(x, y)
#define  extfunction2(x, y)
#define  extfunction3(x, y)
#define  extfunction4(x, y)
#define  extfunction5(x, y)
#define  extfunction6(x, y)
#define  extfunctionn(x, y)
#define  extfunctionq(x, y)
#else
#define  extfunction0(x, y) _extfunction0(x, y, #y)
#define  extfunction1(x, y) _extfunction1(x, y, #y)
#define  extfunction2(x, y) _extfunction2(x, y, #y)
#define  extfunction3(x, y) _extfunction3(x, y, #y)
#define  extfunction4(x, y) _extfunction4(x, y, #y)
#define  extfunction5(x, y) _extfunction5(x, y, #y)
#define  extfunction6(x, y) _extfunction6(x, y, #y)
#define  extfunctionn(x, y) _extfunctionn(x, y, #y)
#define  extfunctionq(x, y) _extfunctionq(x, y, #y)
EXTERN void _extfunction0(const char*, Lispfunction0, const char*);
EXTERN void _extfunction1(const char*, Lispfunction1, const char*);
EXTERN void _extfunction2(const char*, Lispfunction2, const char*);
EXTERN void _extfunction3(const char*, Lispfunction3, const char*);
EXTERN void _extfunction4(const char*, Lispfunction4, const char*);
EXTERN void _extfunction5(const char*, Lispfunction5, const char*);
EXTERN void _extfunction6(const char*, Lispfunction6, const char*);
EXTERN void _extfunctionn(const char*, Lispfunctionn, const char*);
EXTERN void _extfunctionq(const char*, Lispfunctionn, const char*);
#endif

/*** Access macros ***/

#define kar(xx) carfn(varstack, xx)
/* as hd, but safe and handles car(nil)=nil too */
#define kdr(xx) cdrfn(varstack, xx)
/* CDR(X), i.e. as tl(x) but safe and handes cdr(nil)=nil too */
#define keywordp(x) test_flag(dr(x,symbolcell),SYMBOL_IS_KEYWORD)
/* TRUE if Lisp symbol is keyword (starts with ':') */
#define specialp(x) test_flag(dr(x,symbolcell),SYMBOL_IS_SPECIAL)
/* TRUE if Lisp symbol is declared as special variable */

/*** Calling aLisp interpreter from C ***/

EXTERN int a_evaluate(char *forms); /* Returns error code */
EXTERN ohandle eval_forms(bindtype env, char *forms);
EXTERN ohandle evalfn(bindtype, ohandle form);
EXTERN ohandle applyfn(bindtype,ohandle fn,ohandle argl);
EXTERN ohandle call_lisp(ohandle fn,bindtype,int arity, ...);
EXTERN ohandle apply_lisp(ohandle fn,bindtype,int arity,ohandle args[]);
EXTERN ohandle applyarrayfn(bindtype env, ohandle fn, ohandle args);
EXTERN ohandle setfn(bindtype env,ohandle x, ohandle val);
EXTERN int envarity(bindtype);             /* arity of foreign function call */
EXTERN void evalloop(char *prompter);

/// Convenience function for calling a one argument lisp function.
EXTERN ohandle lisp1(const char *fn, ohandle arg);
/// Convenience function for calling a two argument lisp function.
EXTERN ohandle lisp2(const char *fn, ohandle arg, ohandle arg2);
/// Convenience function for calling a three argument lisp function.
EXTERN ohandle lisp3(const char *fn, ohandle arg, ohandle arg2, ohandle arg3);

/*** Error handling ***/

EXTERN ohandle hardresetfn(bindtype);
EXTERN int a_check_reset(bindtype);
typedef ohandle (*catch_function)(bindtype env,void *xa);
EXTERN ohandle a_catch(bindtype env,ohandle label,catch_function fn,void *xa,
                       int *caught);
EXTERN void a_throw(bindtype env, ohandle label, ohandle value);

/*** Debugging ***/

EXTERN void a_setdemon(ohandle loc, int val);
EXTERN int a_setdemonfn(Lispfunction0 fn);
EXTERN void a_backtrace(bindtype bottom, bindtype top);
EXTERN void printframe(bindtype,int);
EXTERN void dumpstack(bindtype,int);
EXTERN size_t frameno(bindtype e);
EXTERN int lsp_traceforms;              /* Traces EVERY aLisp call when TRUE */
EXTERN ohandle a_pgv(char *var);           /* Print global value of variable */

/*** Thread safe CAPI 2.0 interfaces ***/

EXTERN int sa_call_lisp(ohandle *res, sa_connection c, ohandle fn,
                        bindtype env, int arity, ...);
EXTERN int sa_eval_lisp(ohandle *res, sa_connection c, const char *forms);

#endif
