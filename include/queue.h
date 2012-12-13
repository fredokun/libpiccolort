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

struct PICC_QueueCell {
	PICC_PiThread *thread;
	PICC_QueueCell *next;
};

struct PICC_Queue {
	PICC_QueueCell *head;
	PICC_QueueCell *tail;
	int size;
};

struct PICC_ReadyQueue {
	PICC_Queue q;
	PICC_Mutex lock;
};

struct PICC_WaitQueue {
	PICC_Queue active;
	PICC_Queue old;
	PICC_Mutex lock;
};

extern void PICC_ready_queue_push(PICC_ReadyQueue *rq, PICC_PiThread *pt, PICC_Error *error);
extern void PICC_ready_queue_add(PICC_ReadyQueue *rq, PICC_PiThread *pt, PICC_Error *error);
extern PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq);
extern int PICC_ready_queue_size(PICC_ReadyQueue *rq);
extern void PICC_wait_queue_push(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error);
extern PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, PICC_PiThread *pt);
extern void PICC_wait_queue_push_old(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error);
extern PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq);
extern int PICC_wait_queue_size(PICC_WaitQueue *wq);
extern int PICC_wait_queue_max_active(PICC_WaitQueue *wq);
extern void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq);

#endif
