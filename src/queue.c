/**
 * @file queue.c
 * Concurrent ready and wait queues implementation
 *
 * This project is released under MIT License.
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
PICC_QueueCell *PICC_create_queue_cell(PICC_Error *error)
{
	PICC_QueueCell *cell = malloc(sizeof(PICC_QueueCell));
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
void PICC_ready_queue_push(PICC_ReadyQueue *rq, PICC_PiThread *pt, PICC_Error *error)
{
	ASSERT(rq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(rq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

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
void PICC_ready_queue_add(PICC_ReadyQueue *rq, PICC_PiThread *pt, PICC_Error *error)
{
	ASSERT(rq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(rq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

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
PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq)
{
	ASSERT(rq != NULL);

	LOCK_QUEUE(rq);
	PICC_PiThread *popped_thread = NULL;

	if (rq->q.size > 0) {
		PICC_QueueCell *popped_cell = rq->q.head;
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
int PICC_ready_queue_size(PICC_ReadyQueue *rq)
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
void PICC_wait_queue_push(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error)
{
	ASSERT(wq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(wq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

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


PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, PICC_PiThread *pt)
{
	ASSERT(wq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(wq);

	int zone = ACTIVE;
	PICC_PiThread *current = wq->active.head;
	PICC_PiThread *prev = NULL;

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

void PICC_wait_queue_push_old(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error)
{
	ASSERT(wq != NULL);
	ASSERT(pt != NULL);

	LOCK_QUEUE(wq);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

	if (HAS_ERROR) {
		ADD_ERROR(error, cell_error, ERR_WAIT_QUEUE_PUSH_OLD);
	} else {
		cell->thread = pt;
		if (wq->old.size == 0) {
			wq->old.head = cell;
			wq->old.tail = cell;
			cell->next = NULL;
		} else {
			cell->next = wq->old.head;
			wq->old.head = cell;
		}

		if (wq->active.size != 0)
			wq->active.tail->next = cell;

		wq->old.size++;
	}

	UNLOCK_QUEUE(wq);
}

// WARNING: WHY THE POP IS TAKING THE END OF THE QUEUE AND NOT THE HEAD ?
PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq)
{
	ASSERT(wq != NULL);

	LOCK_QUEUE(wq);
	PICC_PiThread *popped_thread = NULL;

	if (wq->old.size > 0) {
		PICC_QueueCell *popped_cell = wq->old.tail;
		popped_thread = popped_cell->thread;
		wq->old.tail = popped_cell->prev;
		wq->old.size--;
	}

	UNLOCK_QUEUE(wq);
	return popped_thread;
}

int PICC_wait_queue_size(PICC_WaitQueue *wq)
{
	ASSERT(wq != NULL);
	LOCK_QUEUE(wq);
	int size = wq->active.size;
	size += wq->old.size;
	UNLOCK_QUEUE(wq);
	return size;
}

int PICC_wait_queue_max_active(PICC_WaitQueue *wq)
{
	ASSERT(wq != NULL);
	LOCK_QUEUE(wq);
	int nb = wq->active.size;
	UNLOCK_QUEUE(wq);
	return nb;
}

void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq)
{
	ASSERT(wq != NULL);
	LOCK_QUEUE(wq);

	wq->old.size = wq->old.size + wq->active.size;
	wq->active.size = 0;
	wq->old.head = wq->active.head;
	wq->active.head = NULL;
	wq->active.tail = NULL;

	UNLOCK_QUEUE(wq);
}
