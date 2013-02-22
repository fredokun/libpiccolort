/**
 * @file queue_repr.h
 * Concurrent ready and wait queues implementation
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef QUEUE_REPR_H
#define QUEUE_REPR_H

#include <queue.h>
#include <concurrent.h>
#include <error.h>

/**
 * The standard PiThread queue cell type
 */
struct _PICC_QueueCell {
    struct _PICC_PiThread *thread;
    struct _PICC_QueueCell *next;
};

/**
 * The standard PiThread queue type
 */
struct _PICC_Queue {
    PICC_QueueCell *head;
    PICC_QueueCell *tail;
    int size;
};

/**
 * The ready PiThread queue type
 */
struct _PICC_ReadyQueue {
    PICC_Queue q;
    PICC_Lock lock;
};

/**
 * The wait PiThread queue type
 */
struct _PICC_WaitQueue {
    PICC_Queue active;
    PICC_Queue old;
    PICC_Lock lock;
};

extern PICC_ReadyQueue *PICC_create_ready_queue(PICC_Error *error);
extern struct _PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq);
extern int PICC_ready_queue_size(PICC_ReadyQueue *rq);

extern PICC_WaitQueue *PICC_create_wait_queue(PICC_Error *error);
extern struct _PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, struct _PICC_PiThread *pt);
extern void PICC_wait_queue_push_old(PICC_WaitQueue *wq, struct _PICC_PiThread *pt, PICC_Error *error);
extern struct _PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq);
extern int PICC_wait_queue_size(PICC_WaitQueue *wq);
extern int PICC_wait_queue_max_active(PICC_WaitQueue *wq);
extern void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq);

// Queue structures invariants
extern void PICC_Queue_inv(PICC_Queue *queue);
extern void PICC_QueueCell_inv(PICC_QueueCell *cell);
extern void PICC_ReadyQueue_inv(PICC_ReadyQueue *queue);
extern void PICC_WaitQueue_inv(PICC_WaitQueue *queue);

#endif
