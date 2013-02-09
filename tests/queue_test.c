/**
 * @file queue_test.c
 * Unit testing of queues.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <pi_thread.h>
#include <queue.h>
#include <tools.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))

/**
 * Creates a stub PiThread
 *
 * @return Stub PiThread
 */
PICC_PiThread *create_stub_thread()
{
    PICC_ALLOC_CRASH(thread, PICC_PiThread) {}
    return thread;
}

void test_ready_queue_push(PICC_Error *error)
{
    PICC_ReadyQueue *q = PICC_create_ready_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();

    PICC_ready_queue_push(q, pt1);
    ASSERT(q->q.size == 1);
    ASSERT(q->q.head->thread == pt1);
    ASSERT(q->q.tail->thread == pt1);

    PICC_ready_queue_push(q, pt2);
    ASSERT(q->q.size == 2);
    ASSERT(q->q.head->thread == pt2);
    ASSERT(q->q.tail->thread == pt1);
}

void test_ready_queue_add(PICC_Error *error)
{
    PICC_ReadyQueue *q = PICC_create_ready_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();

    PICC_ready_queue_add(q, pt1);
    ASSERT(q->q.size == 1);
    ASSERT(q->q.head->thread == pt1);
    ASSERT(q->q.tail->thread == pt1);

    PICC_ready_queue_add(q, pt2);
    ASSERT(q->q.size == 2);
    ASSERT(q->q.head->thread == pt1);
    ASSERT(q->q.tail->thread == pt2);
}

void test_ready_queue_pop(PICC_Error *error)
{
    PICC_ReadyQueue *q = PICC_create_ready_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();

    PICC_ready_queue_add(q, pt1);
    PICC_ready_queue_add(q, pt2);
    ASSERT(q->q.size == 2);
    ASSERT(q->q.head->thread == pt1);

    PICC_PiThread *pt = PICC_ready_queue_pop(q);
    ASSERT(pt == pt1);
    ASSERT(q->q.size == 1);
}

void test_ready_queue_size(PICC_Error *error)
{
    PICC_ReadyQueue *q = PICC_create_ready_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();
    ASSERT(q->q.size == 0);
    PICC_ready_queue_add(q, pt1);
    ASSERT(q->q.size == 1);
    PICC_ready_queue_add(q, pt2);
    ASSERT(q->q.size == 2);
    PICC_ready_queue_pop(q);
    ASSERT(q->q.size == 1);
    PICC_ready_queue_pop(q);
    ASSERT(q->q.size == 0);
}

void test_wait_queue_push(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();

    PICC_wait_queue_push(q, pt1);
    ASSERT(q->old.size == 0);
    ASSERT(q->active.size == 1);
    ASSERT(q->active.head->thread == pt1);
    ASSERT(q->active.tail->thread == pt1);

    PICC_wait_queue_push(q, pt2);
    ASSERT(q->old.size == 0);
    ASSERT(q->active.size == 2);
    ASSERT(q->active.head->thread == pt2);
    ASSERT(q->active.tail->thread == pt1);
}

void test_wait_queue_fetch(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    PICC_PiThread *pt = NULL;
    ASSERT_NO_ERROR();

    pt = PICC_wait_queue_fetch(q, pt1);
    ASSERT(pt == NULL);

    PICC_wait_queue_push(q, pt1);
    ASSERT(q->active.size == 1);
    pt = PICC_wait_queue_fetch(q, pt1);
    ASSERT(pt == pt1);
    ASSERT(q->active.size == 0);
    ASSERT(q->active.head == NULL);
    ASSERT(q->active.tail == NULL);

    PICC_wait_queue_push_old(q, pt2, error);
    ASSERT_NO_ERROR();
    ASSERT(q->old.size == 1);
    pt = PICC_wait_queue_fetch(q, pt1);
    ASSERT(pt == NULL);
    pt = PICC_wait_queue_fetch(q, pt2);
    ASSERT(pt == pt2);
    ASSERT(q->old.size == 0);
    ASSERT(q->old.head == NULL);
    ASSERT(q->old.tail == NULL);
}

void test_wait_queue_push_old(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();
    ASSERT(q->old.size == 0);

    PICC_wait_queue_push_old(q, pt1, error);
    ASSERT_NO_ERROR();
    ASSERT(q->old.size == 1);
    ASSERT(q->old.head->thread == pt1);
    ASSERT(q->old.tail->thread == pt1);
    ASSERT(q->old.head->next == NULL);

    PICC_wait_queue_push_old(q, pt2, error);
    ASSERT_NO_ERROR();
    ASSERT(q->old.size == 2);
    ASSERT(q->old.head->thread == pt2);
    ASSERT(q->old.tail->thread == pt1);
    ASSERT(q->old.head->next == q->old.tail);
    ASSERT(q->old.tail->next == NULL);
}

void test_wait_queue_pop_old(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    PICC_PiThread *pt = NULL;
    ASSERT_NO_ERROR();
    ASSERT(q->old.size == 0);

    PICC_wait_queue_push_old(q, pt1, error);
    PICC_wait_queue_push_old(q, pt2, error);
    ASSERT_NO_ERROR();
    ASSERT(q->old.size == 2);
    ASSERT(q->old.head->thread == pt2);
    ASSERT(q->old.tail->thread == pt1);

    pt = PICC_wait_queue_pop_old(q);
    ASSERT(pt == pt1);
    ASSERT(q->old.size == 1);
    ASSERT(q->old.head->thread == pt2);
    ASSERT(q->old.tail->thread == pt2);

    pt = PICC_wait_queue_pop_old(q);
    ASSERT(pt == pt2);
    ASSERT(q->old.size == 0);
    ASSERT(q->old.head == NULL);
    ASSERT(q->old.tail == NULL);
}

void test_wait_queue_size(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();
    ASSERT(PICC_wait_queue_size(q) == 0);

    PICC_wait_queue_push(q, pt1);
    ASSERT(PICC_wait_queue_size(q) == 1);
    PICC_wait_queue_push_old(q, pt2, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_wait_queue_size(q) == 2);
    PICC_wait_queue_pop_old(q);
    ASSERT(PICC_wait_queue_size(q) == 1);
    PICC_wait_queue_fetch(q, pt1);
    ASSERT(PICC_wait_queue_size(q) == 0);
}

void test_wait_queue_max_active(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    ASSERT_NO_ERROR();
    ASSERT(PICC_wait_queue_max_active(q) == 0);

    PICC_wait_queue_push(q, pt1);
    ASSERT(PICC_wait_queue_max_active(q) == 1);
    PICC_wait_queue_push_old(q, pt2, error);
    ASSERT_NO_ERROR();
    ASSERT(PICC_wait_queue_max_active(q) == 1);
    PICC_wait_queue_pop_old(q);
    ASSERT(PICC_wait_queue_max_active(q) == 1);
    PICC_wait_queue_fetch(q, pt1);
    ASSERT(PICC_wait_queue_max_active(q) == 0);
}

void test_wait_queue_max_active_reset(PICC_Error *error)
{
    PICC_WaitQueue *q = PICC_create_wait_queue(error);
    PICC_PiThread *pt1 = create_stub_thread();
    PICC_PiThread *pt2 = create_stub_thread();
    PICC_PiThread *pt3 = create_stub_thread();
    ASSERT_NO_ERROR();

    PICC_wait_queue_push(q, pt1);
    PICC_wait_queue_push(q, pt2);
    PICC_wait_queue_push_old(q, pt3, error);
    ASSERT_NO_ERROR();
    ASSERT(q->active.size == 2);
    ASSERT(q->old.size == 1);

    PICC_wait_queue_max_active_reset(q);
    ASSERT(q->active.size == 0);
    ASSERT(q->old.size = 3);
    ASSERT(q->active.head == NULL);
    ASSERT(q->active.tail == NULL);
    ASSERT(q->old.head->thread == pt2);
}

/**
 * Runs all queue tests.
 */
void PICC_test_queue()
{
    ALLOC_ERROR(error);
    test_ready_queue_push(&error);
    test_ready_queue_add(&error);
    test_ready_queue_pop(&error);
    test_ready_queue_size(&error);
    test_wait_queue_push(&error);
    test_wait_queue_fetch(&error);
    test_wait_queue_push_old(&error);
    test_wait_queue_pop_old(&error);
    test_wait_queue_size(&error);
    test_wait_queue_max_active(&error);
    test_wait_queue_max_active_reset(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}

