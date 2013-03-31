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

// Forward declarations
struct _PICC_PiThread;

/**
 * The standard PiThread queue cell type
 */
typedef struct _PICC_QueueCell PICC_QueueCell;

/**
 * The standard PiThread queue type
 */
typedef struct _PICC_Queue PICC_Queue;

/**
 * The ready PiThread queue type
 */
typedef struct _PICC_ReadyQueue PICC_ReadyQueue;

/**
 * The wait PiThread queue type
 */
typedef struct _PICC_WaitQueue PICC_WaitQueue;

extern void PICC_ready_queue_push(PICC_ReadyQueue *rq, PICC_PiThread *pt);
extern void PICC_ready_queue_add(PICC_ReadyQueue *rq, PICC_PiThread *pt);
extern void PICC_wait_queue_push(PICC_WaitQueue *wq, PICC_PiThread *pt);

extern void PICC_free_queue(PICC_Queue *q);
extern void PICC_free_wait_queue(PICC_WaitQueue *wq);
extern void PICC_free_ready_queue(PICC_ReadyQueue *rq);

#endif
