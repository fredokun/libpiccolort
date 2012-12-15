/**
 * @file queue.h
 * Concurrent ready and wait queues implementation
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */


#ifndef QUEUE_H
#define QUEUE_H

#include <definitions.h>
#include <error.h>

typedef struct _PICC_QueueCell {
    PICC_PiThread *thread;
    PICC_QueueCell *next;
} PICC_QueueCell;

typedef struct _PICC_Queue {
    PICC_QueueCell *head;
    PICC_QueueCell *tail;
    int size;
} PICC_Queue;

typedef struct _PICC_ReadyQueue {
    PICC_Queue q;
    PICC_Mutex lock;
} PICC_ReadyQueue;

typedef struct _PICC_WaitQueue {
    PICC_Queue active;
    PICC_Queue old;
    PICC_Mutex lock;
} PICC_WaitQueue;

extern PICC_ReadyQueue *PICC_create_ready_queue(PICC_Error *error);
extern void PICC_ready_queue_push(PICC_ReadyQueue *rq, PICC_PiThread *pt, PICC_Error *error);
extern void PICC_ready_queue_add(PICC_ReadyQueue *rq, PICC_PiThread *pt, PICC_Error *error);
extern PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq);
extern int PICC_ready_queue_size(PICC_ReadyQueue *rq);

extern PICC_WaitQueue *PICC_create_wait_queue(PICC_Error *error);
extern void PICC_wait_queue_push(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error);
extern PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, PICC_PiThread *pt);
extern void PICC_wait_queue_push_old(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error);
extern PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq);
extern int PICC_wait_queue_size(PICC_WaitQueue *wq);
extern int PICC_wait_queue_max_active(PICC_WaitQueue *wq);
extern void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq);

#endif
