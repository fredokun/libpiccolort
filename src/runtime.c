/**
 * @file runtime.c
 * File that contains definitions of the prototypes specified in runtime.h
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Dany SIRIPHOL
 * @author Joel HING
 * @author Mickaël MENU
 */

#include <pthread.h>
#include <runtime.h>
#include <queue.h>

/**
 * Temporary main entry point.
 */
int main() {
    ALLOC_ERROR(error);
    NEW_ERROR(&error, ERR_NOT_IMPLEMENTED);
    CRASH(&error);
}

/**
 * Second generation garbage collector.
 *
 * @param schedpool The schedpool to clean
 * @param error Error stack
 * @return ? ????????????????
 */
int PICC_GC2(PICC_SchedPool *sp, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return 0;
}

/**
 * The entry point of the Runtime library. Initialises the real running
 * threads and starts the scheduler
 *
 * @param nb_core_threads Maximum number of core threads that can run at the same time
 * @param entrypoint Entry procedure the for the first thread
 */
void PICC_main(int nb_core_threads, PICC_PiThreadProc entrypoint)
{
    // contains all the errors
    ALLOC_ERROR(error);

    PICC_SchedPool *sched_pool = PICC_create_sched_pool(&error);
    if (HAS_ERROR(error)) CRASH(&error);
    PICC_Args *args = PICC_create_args(sched_pool, &error, &error);
    if (HAS_ERROR(error)) CRASH(&error);

    int i, status;
    void *function = PICC_sched_pool_slave;
    pthread_t *threads = malloc(nb_core_threads * sizeof(pthread_t));
    if (threads == NULL) {
        NEW_ERROR(&error, ERR_OUT_OF_MEMORY);
        CRASH(&error);
    }

    sched_pool->running = true;
    for (i = 0; i < nb_core_threads; i++) {
        status = pthread_create(&threads[i], NULL, function, args);
        if (status) {
            NEW_ERROR(&error, ERR_PTHREAD_CREATE);
            CRASH(&error);
        }
        sched_pool->nb_slaves++;
    }

    // while (sched_pool.nb_waiting_slaves != sched_pool.nb_slaves) {
    //     status = pthread_yield();
    //     if (status)
    //     {
    //         NEW_ERROR(&error, ERR_READY_QUEUE_PUSH ERR_THREAD_YIELD);
    //         CRASH(&error);
    //     }
    // }

    PICC_PiThread *init_thread = PICC_create_pithread(1, 1, &error);
    if (HAS_ERROR(error)) CRASH(&error);
    init_thread->proc = entrypoint;

    PICC_ready_queue_push(sched_pool->ready, init_thread, &error);
    if (HAS_ERROR(error)) CRASH(&error);

    PICC_sched_pool_master(sched_pool, 2, 2, 2, &error);
    if (HAS_ERROR(error)) CRASH(&error);
}
