#ifndef QUEUE_H
#define QUEUE_H

/* Necessary to not include pi_thread.h */

#include <definitions.h>

struct PIT_QueueCell {
	PIT_PiThread *thread;
	PIT_QueueCell *next;
};

struct PIT_Queue {
	PIT_QueueCell *head;
	PIT_QueueCell *tail;
	int size;
};

struct PIT_ReadyQueue {
	PIT_Queue q;
	PIT_Mutex lock;
};

struct PIT_WaitQueue {
	PIT_Queue active;
	PIT_Queue old;
	PIT_Mutex lock;
};

#endif /* QUEUE_H */
