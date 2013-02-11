/**
 * @file queue.c
 * Concurrent ready and wait queues implementation
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#include <stdlib.h>
#include <stdbool.h>
#include <queue.h>
#include <tools.h>

#define LOCK_QUEUE(q) \
    PICC_acquire(&(q->lock));

#define RELEASE_QUEUE(q) \
    PICC_release(&(q->lock));

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
        NEW_ERROR(error, ERR_OUT_OF_MEMORY);
    } else {
        cell->thread = NULL;
        cell->next = NULL;
    }

    #ifdef CONTRACT
        // inv@post
        PICC_QueueCell_inv(cell);
    #endif

    return cell;
}

// READY QUEUES ////////////////////////////////////////////////////////////////

/**
 * Creates a new empty ready queue.
 *
 * @param error Error stack
 */
PICC_ReadyQueue *PICC_create_ready_queue(PICC_Error *error)
{
    PICC_ReadyQueue *queue = malloc(sizeof(PICC_ReadyQueue));
    if (queue == NULL) {
        NEW_ERROR(error, ERR_OUT_OF_MEMORY);
    } else {
        queue->q.head = NULL;
        queue->q.tail = NULL;
        queue->q.size = 0;
    }

    #ifdef CONTRACT
        // inv@post
        PICC_ReadyQueue_inv(queue);
    #endif

    return queue;
}

/**
 * Pushes a PiThread on the given ready queue.
 *
 * @pre rq != null and pt != null
 * @pre pt not in rq
 * @post pt in rq
 * @post rq.size == rq.size@pre + 1
 * @post rq.head.thread == pt
 * @param rq Ready queue
 * @param pt PiThread
 */
void PICC_ready_queue_push(PICC_ReadyQueue *rq, PICC_PiThread *pt)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_ReadyQueue_inv(rq);
        // PICC_PiThread_inv(pt);

        // pre: rq != null && pt != null
        ASSERT(rq != NULL);
        ASSERT(pt != NULL);

        // pre: pt not in rq
        bool found = false;
        PICC_QueueCell *c = rq->q.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            c = c->next;
        }
        ASSERT(found == false);

        // captures
        int size_at_pre = rq->q.size;
    #endif

    LOCK_QUEUE(rq);
    ALLOC_ERROR(error);
    // create queue cell
    ALLOC_ERROR(cell_error);
    PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

    if (HAS_ERROR(cell_error)) {
        ADD_ERROR(&error, cell_error, ERR_READY_QUEUE_PUSH);
    } else {
        cell->thread = pt;
        if (rq->q.size == 0) {
            rq->q.head = cell;
            rq->q.tail = cell;
            cell->next = NULL;
        } else {
            cell->next = rq->q.head;
            rq->q.head = cell;
        }

        rq->q.size++;
    }

    if (HAS_ERROR(error))
        CRASH(&error);

    RELEASE_QUEUE(rq);

    #ifdef CONTRACT
        // inv@post
        PICC_ReadyQueue_inv(rq);
        // PICC_PiThread_inv(pt);

        // post: pt in rq
        found = false;
        c = rq->q.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            c = c->next;
        }
        ASSERT(found == true);
        // post: rq.size == rq.size@pre + 1
        ASSERT(rq->q.size == size_at_pre + 1)
        // post: rq.head.thread == pt
        ASSERT(rq->q.head->thread == pt);
    #endif
}

/**
 * Adds a PiThread at the end of the given ready queue.
 *
 * @pre rq != null && pt != null
 * @pre pt not in rq
 * @post pt in rq
 * @post rq.size == rq.size@pre + 1
 * @post rq.tail.thread == pt
 * @param rq Ready queue
 * @param pt PiThread
 */
void PICC_ready_queue_add(PICC_ReadyQueue *rq, PICC_PiThread *pt)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_ReadyQueue_inv(rq);
        // PICC_PiThread_inv(pt);

        // pre: rq != null && pt != null
        ASSERT(rq != NULL);
        ASSERT(pt != NULL);

        // pre: pt not in rq
        bool found = false;
        PICC_QueueCell *c = rq->q.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            c = c->next;
        }
        ASSERT(found == false);

        // captures
        int size_at_pre = rq->q.size;
    #endif

    LOCK_QUEUE(rq);
    ALLOC_ERROR(error);

    // create queue cell
    ALLOC_ERROR(cell_error);
    PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

    if (HAS_ERROR(cell_error)) {
        ADD_ERROR(&error, cell_error, ERR_READY_QUEUE_ADD);
    } else {
        cell->thread = pt;
        if (rq->q.size == 0) {
            rq->q.head = cell;
            rq->q.tail = cell;
            cell->next = NULL;
        } else {
            rq->q.tail->next = cell;
            rq->q.tail = cell;
            cell->next = NULL;
        }

        rq->q.size++;
    }

    RELEASE_QUEUE(rq);

    if (HAS_ERROR(error))
        CRASH(&error);

    #ifdef CONTRACT
        // inv@post
        PICC_ReadyQueue_inv(rq);
        // PICC_PiThread_inv(pt);

        // post: pt in rq
        found = false;
        c = rq->q.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            c = c->next;
        }
        ASSERT(found == true);
        // post: rq.size == rq.size@pre + 1
        ASSERT(rq->q.size == size_at_pre + 1)
        // post: rq.tail.thread == pt
        ASSERT(rq->q.tail->thread == pt);
    #endif
}

/**
 * Pops a PiThread from the given ready queue.
 *
 * @pre rq != null
 * @post if (rq@pre.size == 0) then NULL
 * @post if (rq@pre.size > 0) then rq@pre.head.thread
 * @post if (rq@pre.size > 0) then rq.size == rq@pre.size - 1
 * @param rq Ready queue
 * @return Popped PiThread
 */
PICC_PiThread *PICC_ready_queue_pop(PICC_ReadyQueue *rq)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_ReadyQueue_inv(rq);

        // pre: rq != null
        ASSERT(rq != NULL);

        // captures
        int size_at_pre = rq->q.size;
        PICC_PiThread *head_at_pre = NULL;
        if (rq->q.head != NULL)
            head_at_pre = rq->q.head->thread;
    #endif

    LOCK_QUEUE(rq);
    PICC_PiThread *popped_thread = NULL;

    if (rq->q.size > 0) {
        PICC_QueueCell *popped_cell = rq->q.head;
        popped_thread = popped_cell->thread;
        rq->q.head = popped_cell->next;
        rq->q.size--;

        if (rq->q.size == 0)
            rq->q.tail = NULL;
    }

    RELEASE_QUEUE(rq);

    #ifdef CONTRACT
        // inv@post
        PICC_ReadyQueue_inv(rq);

        // post: if (rq@pre.size == 0) then NULL
        if (size_at_pre == 0) {
            ASSERT(popped_thread == NULL);
        } else if (size_at_pre > 0) {
            // post: if (rq@pre.size > 0) then rq@pre.head.thread
            ASSERT(popped_thread == head_at_pre);
            // post if (rq@pre.size > 0) then rq.size == rq@pre.size - 1
            ASSERT(rq->q.size == size_at_pre - 1);
        }
    #endif

    return popped_thread;
}

/**
 * Returns the size of the given ready queue.
 *
 * @pre rq != null
 * @post size == SUM(rq.head => rq.tail)
 * @return Size of the ready queue
 */
int PICC_ready_queue_size(PICC_ReadyQueue *rq)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_ReadyQueue_inv(rq);

        // pre: rq != null
        ASSERT(rq != NULL);
    #endif

    LOCK_QUEUE(rq);
    int size = rq->q.size;
    RELEASE_QUEUE(rq);

    #ifdef CONTRACT
        // inv@post
        PICC_ReadyQueue_inv(rq);

        // post: size == SUM(rq.head => rq.tail)
        int count = 0;
        PICC_QueueCell *cell = rq->q.head;
        while (cell != NULL) {
            count++;
            cell = cell->next;
        }
        ASSERT(size == count);
    #endif

    return size;
}

// WAIT QUEUES /////////////////////////////////////////////////////////////////

/**
 * Creates a new empty wait queue.
 *
 * @param error Error stack
 */
PICC_WaitQueue *PICC_create_wait_queue(PICC_Error *error)
{
    PICC_WaitQueue *queue = malloc(sizeof(PICC_WaitQueue));
    if (queue == NULL) {
        NEW_ERROR(error, ERR_OUT_OF_MEMORY);
    } else {
        queue->active.head = NULL;
        queue->active.tail = NULL;
        queue->active.size = 0;

        queue->old.head = NULL;
        queue->old.tail = NULL;
        queue->old.size = 0;
    }

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(queue);
    #endif

    return queue;
}

/**
 * Pushes a PiThread on the given wait queue.
 *
 * @pre wq != null and pt != null
 * @pre pt not in wq.active
 * @pre pt not in wq.old
 * @post pt in wq.active
 * @post wq.active.size == wq.size@pre + 1
 * @post wq.active.head.thread == pt
 * @param wq Wait queue
 * @param pt PiThread
 */
void PICC_wait_queue_push(PICC_WaitQueue *wq, PICC_PiThread *pt)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);
        // PICC_PiThread_inv(pt);

        // pre: wq != null and pt != null
        ASSERT(wq != NULL);
        ASSERT(pt != NULL);

        // pre: pt not in wq.active
        bool found = false;
        PICC_QueueCell *c = wq->active.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            if (c == wq->active.tail)
                break;
            c = c->next;
        }
        ASSERT(found == false);

        // pre: pt not in wq.old
        found = false;
        c = wq->old.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            c = c->next;
        }
        ASSERT(found == false);

        // captures
        int size_at_pre = wq->active.size;
    #endif

    LOCK_QUEUE(wq);
    ALLOC_ERROR(error);

    // create queue cell
    ALLOC_ERROR(cell_error);
    PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

    if (HAS_ERROR(cell_error)) {
        ADD_ERROR(&error, cell_error, ERR_WAIT_QUEUE_PUSH);
    } else {
        cell->thread = pt;
        if (wq->active.size == 0) {
            wq->active.head = cell;
            wq->active.tail = cell;
            cell->next = wq->old.head;
        } else {
            cell->next = wq->active.head;
            wq->active.head = cell;
        }

        wq->active.size++;
    }

    RELEASE_QUEUE(wq);

    if (HAS_ERROR(error))
        CRASH(&error);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);
        // PICC_PiThread_inv(pt);

        // post: pt in wq.active
        found = false;
        c = wq->active.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            if (cell == wq->active.tail)
                break;
            c = c->next;
        }
        ASSERT(found == true);

        // post: wq.active.size == wq.size@pre + 1
        ASSERT(wq->active.size == size_at_pre + 1);
        // post: wq.active.head.thread == pt
        ASSERT(wq->active.head->thread == pt);
    #endif
}


/**
 * Gets a given PiThread from the wait queue.
 *
 * @pre wq != null and pt != null
 * @post if (pt in wq@pre) then pt
 * @post if (pt in wq.active@pre) then (pt not in wq.active && wq.active.size = wq.active@pre.size - 1)
 * @post if (pt in wq.old@pre) then (pt not in wq.old && wq.old.size = wq.old@pre.size - 1)
 * @param wq Wait queue
 * @param pt PiThread
 */
PICC_PiThread *PICC_wait_queue_fetch(PICC_WaitQueue *wq, PICC_PiThread *pt)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);
        // PICC_PiThread_inv(pt);

        // pre: wq != null and pt != null
        ASSERT(wq != NULL);
        ASSERT(pt != NULL);

        // captures
        bool pt_in_active_at_pre = false;
        PICC_QueueCell *c = wq->active.head;
        while (c != NULL) {
            if (c->thread == pt) {
                pt_in_active_at_pre = true;
                break;
            }
            if (c == wq->active.tail)
                break;
            c = c->next;
        }
        bool pt_in_old_at_pre = false;
        c = wq->old.head;
        while (c != NULL) {
            if (c->thread == pt) {
                pt_in_old_at_pre = true;
                break;
            }
            c = c->next;
        }
        bool pt_in_wq_at_pre = pt_in_active_at_pre || pt_in_old_at_pre;
        int active_size_at_pre = wq->active.size;
        int old_size_at_pre = wq->old.size;
    #endif

    LOCK_QUEUE(wq);

    PICC_PiThread *result = NULL;
    int zone = ACTIVE;
    PICC_QueueCell *current = wq->active.head;
    PICC_QueueCell *prev = NULL;

    if (current == NULL) {
        zone = OLD;
        current = wq->old.head;
    }

    while (current != NULL) {
        if (current->thread == pt) {
            if (prev != NULL)
                prev->next = current->next;

            if (zone == ACTIVE) {
                if (current == wq->active.head)
                    wq->active.head = current->next;
                if (current == wq->active.tail)
                    wq->active.tail = prev;
                wq->active.size--;

                if (wq->active.size == 0) {
                    wq->active.head = NULL;
                    wq->active.tail = NULL;
                }

            } else {
                if (current == wq->old.head)
                    wq->old.head = current->next;
                if (current == wq->old.tail)
                    wq->old.tail = prev;
                wq->old.size--;

                if (wq->old.size == 0) {
                    wq->old.head = NULL;
                    wq->old.tail = NULL;
                }
            }

            result = current->thread;
            break;
        }

        prev = current;
        current = current->next;

        if (zone == ACTIVE && current == wq->old.head)
            zone = OLD;
    }

    RELEASE_QUEUE(wq);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);
        // PICC_PiThread_inv(pt);

        if (pt_in_wq_at_pre) {
            // post: if (pt in wq@pre) then pt
            // post: if (pt in wq@pre) then (pt not in wq)
            ASSERT(result == pt);
        }

        // post: if (pt in wq.active@pre) then (pt not in wq.active && wq.active.size = wq.active@pre.size - 1)
        bool found;
        if (pt_in_active_at_pre) {
            found = false;
            c = wq->active.head;
            while (c != NULL) {
                if (c->thread == pt) {
                    found = true;
                    break;
                }
                if (c == wq->active.tail)
                    break;
                c = c->next;
            }
            ASSERT(found == false);
            ASSERT(wq->active.size == active_size_at_pre - 1)
        }

        // post: if (pt in wq.old@pre) then (pt not in wq.old && wq.old.size = wq.old@pre.size - 1)
        if (pt_in_old_at_pre) {
            found = false;
            c = wq->old.head;
            while (c != NULL) {
                if (c->thread == pt) {
                    found = true;
                    break;
                }
                c = c->next;
            }
            ASSERT(found == false);
            ASSERT(wq->old.size == old_size_at_pre - 1)
        }
    #endif

    return result;
}

/**
 * Pushes a PiThread in the old wait queue.
 *
 * @pre wq != null and pt != null
 * @pre pt not in wq
 * @post pt in wq.old
 * @post pt == wq.old.head.thread
 * @post if (wq.old.size@pre > 0) then wq.old.head.next.thread == wq.old.head@pre
 * @post wq.old.size == wq.old.size@pre + 1
 * @param wq Wait queue
 * @param pt PiThread
 * @param error Error stack
 */
void PICC_wait_queue_push_old(PICC_WaitQueue *wq, PICC_PiThread *pt, PICC_Error *error)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);
        // PICC_PiThread_inv(pt);

        // pre: wq != null and pt != null
        ASSERT(wq != NULL);
        ASSERT(pt != NULL);

        // pre: pt not in wq
        bool pt_in_active_at_pre = false;
        PICC_QueueCell *c = wq->active.head;
        while (c != NULL) {
            if (c->thread == pt) {
                pt_in_active_at_pre = true;
                break;
            }
            if (c == wq->active.tail)
                break;
            c = c->next;
        }
        bool pt_in_old_at_pre = false;
        c = wq->old.head;
        while (c != NULL) {
            if (c->thread == pt) {
                pt_in_old_at_pre = true;
                break;
            }
            c = c->next;
        }
        ASSERT(!(pt_in_active_at_pre || pt_in_old_at_pre));

        // captures
        int size_at_pre = wq->old.size;
        PICC_PiThread *head_at_pre = NULL;
        if (wq->old.head != NULL)
            head_at_pre = wq->old.head->thread;
    #endif

    LOCK_QUEUE(wq);

    // create queue cell
    ALLOC_ERROR(cell_error);
    PICC_QueueCell *cell = PICC_create_queue_cell(&cell_error);

    if (HAS_ERROR(cell_error)) {
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

    RELEASE_QUEUE(wq);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);
        // PICC_PiThread_inv(pt);

        // post: pt in wq.old
        bool found = false;
        c = wq->old.head;
        while (c != NULL) {
            if (c->thread == pt) {
                found = true;
                break;
            }
            c = c->next;
        }
        ASSERT(found == true);

        // post: pt == wq.old.head.thread
        ASSERT(pt == wq->old.head->thread);
        // post: if (wq.old.size@pre > 0) then wq.old.head.next.thread == wq.old.head@pre
        if (size_at_pre > 0)
            ASSERT(wq->old.head->next->thread == head_at_pre);
        // post: wq.old.size == wq.old.size@pre + 1
        ASSERT(wq->old.size == size_at_pre + 1);

    #endif
}

/**
 * Pops a PiThread from the old wait queue.
 *
 * WARNING: WHY THE POP IS TAKING THE END OF THE QUEUE AND NOT THE HEAD ?
 *
 * @pre wq != null
 * @post if (wq.old@pre.size == 0) then NULL
 * @post if (wq.old@pre.size > 0) then wq.old@pre.tail.thread
 * @post if (wq.old@pre.size > 0) then wq.old.size == wq.old.size@pre - 1
 * @param wq Wait queue
 */
PICC_PiThread *PICC_wait_queue_pop_old(PICC_WaitQueue *wq)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);

        // pre: wq != null
        ASSERT(wq != NULL);

        // captures
        int size_at_pre = wq->old.size;
        PICC_PiThread *tail_at_pre = NULL;
        if (wq->old.tail != NULL)
            tail_at_pre = wq->old.tail->thread;
    #endif

    LOCK_QUEUE(wq);
    PICC_PiThread *popped_thread = NULL;

    if (wq->old.size > 0) {
        PICC_QueueCell *popped_cell = wq->old.tail;
        popped_thread = popped_cell->thread;

        if (wq->old.size == 1) {
            wq->old.tail = NULL;
            wq->old.head = NULL;
        } else {
            PICC_QueueCell *prev = wq->old.head;
            while (prev != NULL && prev->next != popped_cell) {
                prev = prev->next;
            }

            if (prev != NULL) {
                prev->next = NULL;
                wq->old.tail = prev;
            }
        }
        wq->old.size--;
    }

    RELEASE_QUEUE(wq);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);

        // post: if (wq.old@pre.size == 0) then NULL
        if (size_at_pre == 0) {
            ASSERT(popped_thread == NULL);
        } else if (size_at_pre > 0) {
            // post: if (wq.old@pre.size > 0) then wq.old@pre.tail.thread
            ASSERT(popped_thread == tail_at_pre);
            // post: if (wq.old@pre.size > 0) then wq.old.size == wq.old.size@pre - 1
            ASSERT(wq->old.size == size_at_pre - 1);
        }
    #endif

    return popped_thread;
}

/**
 * Returns the size of the active + old wait queues.
 *
 * @pre wq != null
 * @post size == SUM(wq.active.head => wq.active.tail) + SUM(wq.old.head => wq.old.tail)
 * @param wq Wait queue
 */
int PICC_wait_queue_size(PICC_WaitQueue *wq)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);

        // pre: wq != null
        ASSERT(wq != NULL);
    #endif

    LOCK_QUEUE(wq);
    int size = wq->active.size;
    size += wq->old.size;
    RELEASE_QUEUE(wq);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);

        // post: size == SUM(wq.active.head => wq.active.tail) + SUM(wq.old.head => wq.old.tail)
        int count = 0;
        PICC_QueueCell *cell = wq->active.head;
        while (cell != NULL) {
            count++;
            if (cell == wq->active.tail)
                break;
            cell = cell->next;
        }
        cell = wq->old.head;
        while (cell != NULL) {
            count++;
            cell = cell->next;
        }
        ASSERT(size == count);
    #endif

    return size;
}

/**
 * Returns the size of the active wait queue.
 *
 * @pre wq != null
 * @post size == SUM(wq.active.head => wq.active.tail)
 * @param wq Wait queue
 */
int PICC_wait_queue_max_active(PICC_WaitQueue *wq)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);

        // pre: wq != null
        ASSERT(wq != NULL);
    #endif

    ASSERT(wq != NULL);
    LOCK_QUEUE(wq);
    int size = wq->active.size;
    RELEASE_QUEUE(wq);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);

        // post: size == SUM(wq.active.head => wq.active.tail)
        int count = 0;
        PICC_QueueCell *cell = wq->active.head;
        while (cell != NULL) {
            count++;
            if (cell == wq->active.tail)
                break;
            cell = cell->next;
        }
        ASSERT(size == count);
    #endif

    return size;
}

/**
 * Resets the active wait queue by pushing its cells in the old wait queue.
 *
 * @pre wq != null
 * @post wq.old.size == wq.old.size@pre + wq.active.size@pre
 * @post wq.active.size == 0
 * @post wq.active.head == wq.active.tail == null
 * @post wq.old.head == wq.active.head@pre
 * @param wq Wait queue
 */
void PICC_wait_queue_max_active_reset(PICC_WaitQueue *wq)
{
    #ifdef CONTRACT
        // inv@pre
        PICC_WaitQueue_inv(wq);

        // pre: wq != null
        ASSERT(wq != NULL);

        // captures
        int active_size_at_pre = wq->active.size;
        int old_size_at_pre = wq->old.size;
        PICC_PiThread *active_head_at_pre = NULL;
        if (wq->active.head != NULL)
            active_head_at_pre = wq->active.head->thread;
    #endif

    LOCK_QUEUE(wq);

    wq->old.size = wq->old.size + wq->active.size;
    wq->active.size = 0;
    wq->old.head = wq->active.head;
    wq->active.head = NULL;
    wq->active.tail = NULL;

    RELEASE_QUEUE(wq);

    #ifdef CONTRACT
        // inv@post
        PICC_WaitQueue_inv(wq);

        // post: wq.old.size == wq.old.size@pre + wq.active.size@pre
        ASSERT(wq->old.size == old_size_at_pre + active_size_at_pre);
        // post: wq.active.size == 0
        ASSERT(wq->active.size == 0);
        // post: wq.active.head == wq.active.tail == null
        ASSERT(wq->active.head == NULL);
        ASSERT(wq->active.tail == NULL);
        // post: wq.old.head == wq.active.head@pre
        ASSERT(wq->old.head->thread == active_head_at_pre);
    #endif
}

// Queue structure invariants //////////////////////////////////////////////////

/**
 * Checks queue invariant.
 *
 * @inv size >= 0
 * @inv size == SUM(head => tail)
 * @inv if (size == 0) then (head == tail == NULL)
 * @inv if (size == 1) then (head == tail)
 * @inv if (size > 0) then (head != NULL and tail != NULL)
 */
void PICC_Queue_inv(PICC_Queue *queue)
{
    ASSERT(queue->size >= 0);

    int size = 0;
    PICC_QueueCell *cell = queue->head;
    while (cell != NULL) {
        size++;
        if (cell == queue->tail)
            break;
        cell = cell->next;
    }
    ASSERT(size == queue->size);

    if (queue->size == 0) {
        ASSERT(queue->head == NULL);
        ASSERT(queue->tail == NULL);
    } else if (queue->size == 1) {
        ASSERT(queue->head == queue->tail);
    }

    if (queue->size > 0) {
        ASSERT(queue->head != NULL);
        ASSERT(queue->tail != NULL);
    }
}

/**
 * Checks queue cell invariant.
 *
 * @inv cell != null
 */
void PICC_QueueCell_inv(PICC_QueueCell *cell)
{
    ASSERT(cell != NULL);
}

/**
 * Checks ready queue invariant.
 *
 * @inv check_inv(q)
 */
void PICC_ReadyQueue_inv(PICC_ReadyQueue *queue)
{
    PICC_Queue_inv(&queue->q);
}

/**
 * Checks wait queue invariant.
 *
 * @inv check_inv(active)
 * @inv check_inv(old)
 */
void PICC_WaitQueue_inv(PICC_WaitQueue *queue)
{
    PICC_Queue_inv(&queue->active);
    PICC_Queue_inv(&queue->old);
}
