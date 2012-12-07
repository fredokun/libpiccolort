/**
 * @file queue.c
 * Concurrent ready and wait queues implementation
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <error.h>

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

/**
 * Pushes a new thread on the given ready queue.
 *
 * @param rq Ready queue
 * @param pt PiThread
 */
void PIT_ready_queue_push(PIT_ReadyQueue *rq, PIT_PiThread *pt, PIT_Error *error)
{
	ASSERT(rq != NULL);
	ASSERT(pt != NULL);

	pthread_mutex_lock(&rq.lock);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PIT_QueueCell *cell = PIT_create_queue_cell(&cell_error);

	if (HAS_ERROR) {
		ADD_ERROR(error, cell_error, ERR_READY_QUEUE_PUSH);
	} else {
		cell->thread = pt;
		if (rq->size == 0) {
			rq->head = cell;
			rq->tail = cell;
			cell->next = NULL;
		} else {
			cell->next = rq->head;
			rq->head = cell;
		}

		rq->size++;
	}

	pthread_mutex_unlock(&rq.lock);
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

	pthread_mutex_lock(&rq.lock);

	// create queue cell
	ALLOC_ERROR(cell_error);
	PIT_QueueCell *cell = PIT_create_queue_cell(&cell_error);

	if (HAS_ERROR) {
		ADD_ERROR(error, cell_error, ERR_READY_QUEUE_ADD);
	} else {
		cell->thread = pt;
		if (rq->size == 0) {
			rq->head = cell;
			rq->tail = cell;
			cell->next = NULL;
		} else {
			rq->tail->next = cell;
			rq->tail = cell;
			tail->next = NULL;
		}

		rq->size++;
	}

	pthread_mutex_unlock(&rq.lock);
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

	pthread_mutex_lock(&rq.lock);
	PIT_PiThread *popped_thread = NULL;

	if (rq->size > 0) {
		PIT_QueueCell *popped_cell = rq->head;
		popped_thread = popped_cell->thread;
		rq->head = popped_cell->next;
		rq->size--;
	}

	pthread_mutex_unlock(&rq.lock);
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
	pthread_mutex_lock(&rq.lock);
	int size = rq->size;
	pthread_mutex_unlock(&rq.lock);
	return size;
}
