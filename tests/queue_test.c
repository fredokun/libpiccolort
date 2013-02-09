/**
 * @file queue_test.c
 * Unit testing of queues.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pi_thread.h>
#include <queue.h>
#include <tools.h>

/**
 * Creates a stub PiThread
 *
 * @return Stub PiThread
 */
PICC_PiThread *create_stub_thread()
{
    PICC_ALLOC_CRASH(thread, PICC_PiThread) {}
    return thread;
}


extern void PICC_ready_queue_push(PICC_ReadyQueue *rq, struct _PICC_PiThread *pt);
extern void PICC_ready_queue_add(PICC_ReadyQueue *rq, struct _PICC_PiThread *pt);
extern struct _PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq);
extern int PICC_ready_queue_size(PICC_ReadyQueue *rq);

extern void PICC_wait_queue_push(PICC_WaitQueue *wq, struct _PICC_PiThread *pt);
extern struct _PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, struct _PICC_PiThread *pt);
extern void PICC_wait_queue_push_old(PICC_WaitQueue *wq, struct _PICC_PiThread *pt, PICC_Error *error);
extern struct _PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq);
extern int PICC_wait_queue_size(PICC_WaitQueue *wq);
extern int PICC_wait_queue_max_active(PICC_WaitQueue *wq);
extern void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq);


/**
 * Runs all queue tests.
 */
void PICC_test_queue()
{

}

