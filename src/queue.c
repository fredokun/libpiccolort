/**
 * @file queue.c
 * Concurrent ready and wait queues implementation
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <queue.h>
#include <error.h>

#define LOCK_QUEUE(q) \
	pthread_mutex_lock(&(q->lock));

#define UNLOCK_QUEUE(q) \
	pthread_mutex_unlock(&(q->lock));

// used for wait queue zones
const static int ACTIVE = 1;
const static int OLD = 0;

/**
 * Allocates a new queue cell.
 *
 * @return Allocated queue cell
 */
PIT_QueueCell *PIT_create_queue_cell(PIT_Error *error)
{
	PIT_QueueCell *cell = malloc(sizeof(PIT_QueueCell));
	if (cell == NULL) {
		NEW_ERROR(error, ERR_ALLOC_QUEUE_CELL);
	} else {
		cell->thread = NULL;
		cell->next = NULL;
	}
	return cell;
}

// READY QUEUES ////////////////////////////////////////////////////////////////

/**
 * Pushes a PiThread on the given ready queue.
 *
 * @param rq Ready queue
 * @param pt PiThread
 */
void PIT_ready_queue_push(PIT_ReadyQueue *rq, PIT_PiThread *pt, PIT_Error *error)
{
	ASSERT(rq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(rq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PIT_QueueCell *cell = PIT_create_queue_cell(&cell_error);

	if (HAS_ERROR) {
		ADD_ERROR(error, cell_error, ERR_READY_QUEUE_PUSH);
	} else {
		cell->thread = pt;
		if (rq->q.size == 0) {
			rq->q.head = cell;
			rq->q.tail = cell;
			cell->next = NULL;
		} else {
			cell->next = rq->head;
			rq->q.head = cell;
		}

		rq->q.size++;
	}

	UNLOCK_QUEUE(rq);
}

/**
 * Adds a PiThread at the end of the given ready queue.
 *
 * @param rq Ready queue
 * @param pt PiThread
 */
void PIT_ready_queue_add(PIT_ReadyQueue *rq, PIT_PiThread *pt, PIT_Error *error)
{
	ASSERT(rq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(rq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PIT_QueueCell *cell = PIT_create_queue_cell(&cell_error);

	if (HAS_ERROR) {
		ADD_ERROR(error, cell_error, ERR_READY_QUEUE_ADD);
	} else {
		cell->thread = pt;
		if (rq->q.size == 0) {
			rq->q.head = cell;
			rq->q.tail = cell;
			cell->next = NULL;
		} else {
			rq->q.tail->next = cell;
			rq->q.tail = cell;
			tail->next = NULL;
		}

		rq->q.size++;
	}

	UNLOCK_QUEUE(rq);
}

/**
 * Pops a PiThread from the given ready queue.
 *
 * @param rq Ready queue
 * @return Popped PiThread
 */
PIT_PiThread *PIT_ready_queue_pop(PIT_ReadyQueue *rq)
{
	ASSERT(rq != NULL);

	LOCK_QUEUE(rq);
	PIT_PiThread *popped_thread = NULL;

	if (rq->q.size > 0) {
		PIT_QueueCell *popped_cell = rq->q.head;
		popped_thread = popped_cell->thread;
		rq->q.head = popped_cell->next;
		rq->q.size--;
	}

	UNLOCK_QUEUE(rq);
	return popped_thread;
}

/**
 * Returns the size of the given ready queue.
 *
 * @return Size of the ready queue
 */
int PIT_ready_queue_size(PIT_ReadyQueue *rq)
{
	ASSERT(rq != NULL);
	LOCK_QUEUE(rq);
	int size = rq->q.size;
	UNLOCK_QUEUE(rq);
	return size;
}

// WAIT QUEUES /////////////////////////////////////////////////////////////////

/**
 * Pushes a PiThread on the given wait queue.
 *
 * @param wq Wait queue
 * @param pt PiThread
 */
void PIT_wait_queue_push(PIT_WaitQueue *wq, PIT_PiThread *pt, PIT_Error *error)
{
	ASSERT(wq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(wq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PIT_QueueCell *cell = PIT_create_queue_cell(&cell_error);

	if (HAS_ERROR) {
		ADD_ERROR(error, cell_error, ERR_WAIT_QUEUE_PUSH);
	} else {
		cell->thread = pt;
		if (wq->active.size == 0) {
			wq->active.head = cell;
			wq->active.tail = cell;
			cell->next = wq->old.head;
		} else {
			cell->next = wq->active.head;
			wq->active.head = pt;
		}

		wq->active.size++;
	}

	UNLOCK_QUEUE(wq);
}


PIT_PiThread *PIT_wait_queue_fetch(PIT_WaitQueue *wq, PIT_PiThread *pt, PIT_Error *error)
{
	ASSERT(wq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(wq);

	int zone = ACTIVE;
	PIT_PiThread *current = wq->active.head;
	PIT_PiThread *prev = NULL;

	if (current == NULL) {
		zone = OLD;
		current = wq->old.head;
	}

	while (current != NULL) {
		if (current == pt) {
			if (prev != NULL)
				prev->next = current->next;

			if (zone == ACTIVE) {
				if (current == wq->active.head)
					wq->active.head = current->next;
				if (current == wq->active.tail)
					wq->active.tail = prev;
				wq->active.size--;

			} else {
				if (current == wq->old.head)
					wq->old.head = current->next;
				if (current == wq->old.tail)
					wq->old.tail = prev;
				wq->old.size--;
			}

			UNLOCK_QUEUE(wq);
			return current;
		}

		prev = current;
		current = current->next;

		if (zone == ACTIVE && current == wq->old.head)
			zone = OLD;
	}

	UNLOCK_QUEUE(wq);
	return NULL;
}
