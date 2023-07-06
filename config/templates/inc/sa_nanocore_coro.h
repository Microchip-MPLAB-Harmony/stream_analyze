/*****************************************************************************
 * sa.engine
 *
 * Author: (c) 2023 Johan Risch, SA
 *
 * Description: coroutines for sa.engine nanocore
 ****************************************************************************/

#ifndef _SA_NANOCORE_CORO_
#define _SA_NANOCORE_CORO_
/*
# SA Nanocore coroutine using long jumps.

SA Nanocore can be run indide a while(1) loop by using
this subsystem. It implements coroutines using set_jmp
and long_jmp.

When starting a stack SA Nanocore will do down stack_size
+ stack_slack

```
--- Main
0
1
2
...
stack_size
...
sa_engine_stack_start
stack_size+stack_slack
```


When SA Nanocore resumes it will check the CRC of the stack from stack_size
stack_size+stack_slack and compare it to before yielding. If the checsums
differ the task will go into a broken state and will have to be discarded.

1. Creating the task, how the stack is created:
```
--- Main                               task = CreateTask()
1   CreateTask
    SetJmp <-----------------------+
2   alloca(stack_size+stack+slack) |
3                     +            |
...                   |            |
stack_size+stack_slack|            |
stack_size            |            |
...                   |            |
sa_engine_stack_start<+            |
...                                |
y1 TaskReady                       |
   SetJmp                          |
   long_jmp+-----------------------+
```

Running  subtask is possible as long as the stack needed by the subsystem is less
than stack_size:
```
 --- Main                    while(1) {
 1   SubSystem1()              Subsystem1()<--+
 2                             Subsystem2()
 3                             RunTask(task)
 ...                         }
 stack_size+stack_slack
 stack_size
 ...
 sa_engine_stack_start
 ...
 y1 TaskReady
    SetJmp
    long_jmp


  --- Main                   while(1) {
  1   |                        Subsystem1()
  2   |                        Subsystem2()<--+
  3   SubSystem2()             RunTask(task)
  ...                        }
  stack_size+stack_slac
  stack_size
  ...
  sa_engine_stack_start
  ...
  y1 TaskReady
     SetJmp
     long_jmp

```

When resuming the task a longjump is performed
to allow SA Nanocore to continue where it where.
When resuming a crc of the stack_slack will be done.

When a the yield timmer fires a longjump back to RunTask
will be made. allowing the next iteration in the while(1)
loop.

```
  --- Main                   while(1) {
  1   RunTask +-----------+    Subsystem1()
  2                       |    Subsystem2()
  3                       |    RunTask(task)<--+
  ...                     |  }
  stack_size+stack_slack  |
  stack_size              |
  ...                     |
  sa_engine_stack_start   |
  ...                     |
  y1 TaskReady            |
  +--SetJmp <-------------+
  |  long_jmp
  +->...


  --- Main                     while(1) {
  1   RunTask <------------+     Subsystem1()
  2                        |     Subsystem2()
  3                        |     RunTask(task)<--+
  ...                      |   }
  stack_size+stack_slack   |
  stack_size               |
  ...                      |
  sa_engine_stack_start    |
  ...                      |
  y1 TimerInterrupt        |
     SetJmp                |
     long_jmp  +-----------+


```


*/

 /*******************************************************************************
  Header inclusions
 *******************************************************************************/
#include "sa_syscalls.h"
#include "sa_storage.h"
#include "sa_handle.h"
#include "sa_lisp.h"

 /*******************************************************************************
  Data-types
 *******************************************************************************/
typedef enum task_state_enum
{
    SA_STATUS_FREE = 0,
    SA_STATUS_BROKEN,
    SA_STATUS_RUNNING,
    SA_STATUS_YIELDED,
    SA_STATUS_WAITING,
    SA_STATUS_DONE
} sa_task_state;


typedef struct {
    char *stackptr;
    jmp_buf yield_pos;
    sa_task_state status;
    sa_time_t idle_time_left;
    char stop;
} sa_engine_task;


 /*******************************************************************************
 Public Functions
 *******************************************************************************/
/*! \brief
 * The main init function for SA Engine
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void SAEngine_NanocoreInit(void* register_functions);

/*! \brief Attempts to create a new task running the lisp-form form.
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return: returns -1 if no tasks are available.
 */
extern int create_task(void(*task_fun)(int, void *), const char *form);

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void SAEngine_NanocoreInitTask(int stack_size, int stack_slack);

/*! \brief Run tasks created using create_task
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern sa_task_state run_task(int task);

/*! \brief
 * Run the tasks that have been registered to the SA Engine nannocore
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern sa_task_state SAEngine_NanocoreRunTasks( void );

/*! \brief   Set sa nanocore to start executing forms.  Will only initialize execution, use
    sa_nancore_run to run forms set for execution.
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return: state of nanocore task
 */
extern int sa_nanocore_execute(const char *forms);

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void sa_nanocore_task_yield();

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern long sa_nanocore_stack_left();

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void sa_nanocore_raise_interrupt();

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void sa_nanocore_finalize_task(int taskid);

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern int sa_nanocore_task_is_stopped();

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void sa_nanocore_set_sleep_time(sa_time_t micros);

/*! \brief  Run current sa nancore forms
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return: state of nanocore task
 */
extern sa_task_state sa_nanocore_run();

/*! \brief  Abort current running sa nanocore form.
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern int sa_nanocore_abort();

/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern int SAEngine_NanocoreInitEvalLoop( void );


/*! \brief
 *
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void SAEngine_NanocoreInitCOM( void);

/*! \brief
 * Read function for communication over a serial interface
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void SAEngine_SerialBufferRead(char*, int);

/*! \brief
 *  System ticker needed for keeping the serial interface alive
 * Details.
 *
 *
 * @param[in]:
 * @param[in/out]:
 * @param[out]:
 * @return:
 */
extern void SAEngine_TimeoutHandlerTick(uintptr_t context);

#endif
