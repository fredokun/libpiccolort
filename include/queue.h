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

#include <pi_thread.h>
#include <sync.h>
#include <error.h>

/**
 * The standard PiThread queue cell type
 */
typedef struct _PICC_QueueCell {
    /**@{*/
    struct _PICC_PiThread *thread; /** The pi-thread contained in the
                                        cell */
    struct _PICC_QueueCell *next; /** A pointer to the next cell or
                                        NULL if it does not exist */
    /**@}*/
} PICC_QueueCell;

/**
 * The standard PiThread queue type
 */
typedef struct _PICC_Queue {
    /**@{*/
    PICC_QueueCell *head; /**< The head of the queue */
    PICC_QueueCell *tail; /**< The tail of the queue */
    int size; /**< The size of the queue */
    /**@}*/
} PICC_Queue;

/**
 * The ready PiThread queue type
 */
typedef struct _PICC_ReadyQueue {
    /**@{*/
    PICC_Queue q; /** The standard PiThread queue that contains all
                    the data */
    PICC_Mutex lock; /** The mutex used to sinchronize the data with
                        the other threads */
    /**@}*/
} PICC_ReadyQueue;

/**
 * The wait PiThread queue type
 */
typedef struct _PICC_WaitQueue {
    /**@{*/
    PICC_Queue active; /** The standard queue that contains the
                        PiThreads that are not waiting for a long time */
    PICC_Queue old; /** The standard queue that contains the PiThreads
                        that are waiting a lot of time */
    PICC_Mutex lock; /** The mutex used to sinchronize the queues with
                        the other threads */
    /**@}*/
} PICC_WaitQueue;

extern PICC_ReadyQueue *PICC_create_ready_queue(PICC_Error *error);
extern void PICC_ready_queue_push(PICC_ReadyQueue *rq, struct _PICC_PiThread *pt, PICC_Error *error);
extern void PICC_ready_queue_add(PICC_ReadyQueue *rq, struct _PICC_PiThread *pt);
extern struct _PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq);
extern int PICC_ready_queue_size(PICC_ReadyQueue *rq);

extern PICC_WaitQueue *PICC_create_wait_queue(PICC_Error *error);
extern void PICC_wait_queue_push(PICC_WaitQueue *wq, struct _PICC_PiThread *pt);
extern struct _PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, struct _PICC_PiThread *pt);
extern void PICC_wait_queue_push_old(PICC_WaitQueue *wq, struct _PICC_PiThread *pt, PICC_Error *error);
extern struct _PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq);
extern int PICC_wait_queue_size(PICC_WaitQueue *wq);
extern int PICC_wait_queue_max_active(PICC_WaitQueue *wq);
extern void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq);

#endif
