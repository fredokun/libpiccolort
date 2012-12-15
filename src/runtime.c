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

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <queue.h>
#include <definitions.h>
#include <runtime.h>
#include <pi_thread.h>

/**
 * The entry point of the Runtime library. Initialises the real running
 * threads and starts the scheduler
 *
 * @param nb_core_threads the maximum number of core threads that can
 * rum at the same time
 * @param entrypoint the entry procedure the for the first thread
 */
void PICC_main(int nb_core_threads, PICC_PiThreadProc entrypoint)
{
    ALLOC_ERROR(error); /* Contains all the errors */
    PICC_SchedPool *sched_pool = PICC_create_sched_pool(&error);
    if (HAS_ERROR(error))
    {
        CRASH(&error);
    }
    PICC_PiThread *init_thread;
    PICC_Args * args = (PICC_Args *)malloc(sizeof(PICC_Args));
    args->error = error;
    args->sched_pool = sched_pool;
    void* function = PICC_sched_pool_slave;
    int i;
    pthread_t *threads;

    int status;

    threads = (pthread_t *)malloc(nb_core_threads * sizeof(pthread_t));
    if (threads == NULL)
    {
        NEW_ERROR(&error, ERR_OUT_OF_MEMORY);
        CRASH(&error);
    }

    sched_pool->running = true;


    for (i = 0; i < nb_core_threads; ++i)
    {
        status = pthread_create(&threads[i], NULL,
            function, args);
        if (status)
        {
            NEW_ERROR(&error, ERR_THREAD_CREATE);
            CRASH(&error);
        }
        ++(sched_pool->nb_slaves);
    }

/*
    while (sched_pool.nb_waiting_slaves != sched_pool.nb_slaves) {
        status = pthread_yield();
        if (status)
        {
            NEW_ERROR(&error, ERR_READY_QUEUE_PUSH ERR_THREAD_YIELD);
            CRASH(&error);
        }
    }
*/

    init_thread = PICC_create_pithread();
    init_thread->proc = entrypoint;

    PICC_ready_queue_push(sched_pool->ready, init_thread, &error);
    if (HAS_ERROR(error))
        CRASH(&error);

    PICC_sched_pool_master(*sched_pool, 2, 2, 2);
}



/**
 * Create and init an atomic boolean
 *
 * @return created atomic boolean
 */
PICC_AtomicBoolean PICC_create_atomic_boolean()
{
    PICC_AtomicBoolean ab;
    PICC_init_mutex(ab.lock);
    ab.value = false;
    return ab;
}

/**
 * Second generation garbage collector.
 *
 * @param schedpool The schedpool to clean.
 * @return ? ????????????????
 */
int PICC_GC2(PICC_SchedPool schedpool)
{
    printf("Not implemented yet.\n");
    return 0;
}

/**
 * Creates a new scheduler
 *
 * @param error the error container
 *
 * @return the created scheduler structure
 */
PICC_SchedPool *PICC_create_sched_pool(PICC_Error *error)
{
    PICC_SchedPool *pool = (PICC_SchedPool *)malloc(sizeof(struct PICC_SchedPool));
    if (pool == NULL) {
        NEW_ERROR(error, ERR_OUT_OF_MEMORY);
    } else {
		ALLOC_ERROR(alloc1_error);
		pool->ready = PICC_create_ready_queue(&alloc1_error);
		if (HAS_ERROR(alloc1_error)) {
			ADD_ERROR(error, alloc1_error, ERR_OUT_OF_MEMORY);
		}
		ALLOC_ERROR(alloc2_error);
		pool->wait = PICC_create_wait_queue(&alloc2_error);
		if (HAS_ERROR(alloc2_error)) {
			ADD_ERROR(error, alloc2_error, ERR_OUT_OF_MEMORY);
		}
		pool->nb_slaves = pool->nb_waiting_slaves = 0;
	}
    return pool;
}



/**
 * Function that creates a PICC_PiThread.
 * @return PICC_PiThread a fresh new created PiThread
 */
PICC_PiThread *PICC_create_pithread()
{
    PICC_PiThread *new_thread = (PICC_PiThread*)malloc(sizeof(PICC_PiThread));
    new_thread->knowns = (PICC_KnownsSet)malloc(sizeof(PICC_KnownsSet));
    new_thread->fuel = FUEL_INIT;
    return new_thread;
}

/**
 * Function that creates a clock
 * @return PICC_Clock a fresh new created clock
 */
PICC_Clock *PICC_create_clock()
{
    PICC_Clock * new_clock = (PICC_Clock*)malloc(sizeof(PICC_Clock));
    return new_clock;
}

/**
 * Function that handle the behavior of a sched_pool
 * @param sched_pool the sched_pool to manage
 */
void PICC_sched_pool_slave(PICC_Args *args)
{
    PICC_SchedPool *sched_pool = args->sched_pool;
    PICC_Error *error = &(args->error);

    PICC_PiThread *current;

    while(sched_pool->running)
    {
        while(PICC_ready_queue_size(sched_pool->ready))
        {
            current = PICC_ready_queue_pop(sched_pool->ready);
            do
            {
                current->proc(*sched_pool, *current);
            } while(current->status == PICC_STATUS_CALL);

            if(current->status == PICC_STATUS_BLOCKED) // && safe_choice
                NEW_ERROR(error,ERR_DEADLOCK);
        }

        PICC_acquire(sched_pool->lock);
        sched_pool->nb_waiting_slaves++;
        PICC_cond_wait(sched_pool->cond, sched_pool->lock);
        sched_pool->nb_waiting_slaves--;
        PICC_release(sched_pool->lock);
    }
}

/**
 * ????????????????????
 * @param sched_pool ????????????????
 * @param std_gc_fuel ?????????????
 * @param quick_gc_fuel ????????????
 * @param active_factor ?????????????
 */
void PICC_sched_pool_master(PICC_SchedPool sched_pool, int std_gc_fuel, int quick_gc_fuel, int active_factor)
{
    printf("Not implemented yet.\n");
    return;
}


/**
 * Function that register an output commitment according to pithread and channel
 * @param p PiThread
 * @param c PICC_Commit
 * @return ? ?????????????
 */
int PICC_can_awake(PICC_PiThread p, PICC_Commit c)
{
    printf("Not implemented yet.\n");
    return 0;
}

/**
 * ?????????????
 * @param sched ???
 * @param p ????
 * @return ? ?????????????
 */
void PICC_awake(PICC_SchedPool sched, PICC_PiThread p)
{
    printf("Not implemented yet.\n");
    return;
}

/

/**
 * Function that creates the PICC_CommitList
 * @return the created PICC_CommitList
 */
PICC_CommitList *PICC_create_commit_list()
{
    PICC_CommitList *new_commit_list = (PICC_CommitList*)malloc(sizeof(PICC_CommitList));
    new_commit_list->head = NULL;
    new_commit_list->tail = NULL;
    new_commit_list->size = 0;
    return new_commit_list;
}

/**
 * Function that creates the PICC_CommitListElement, an element of the PICC_CommitList
 * @return the created element PICC_CommitListElement, an element of the PICC_CommitList
 */
PICC_CommitListElement *PICC_create_commit_list_element(PICC_Commit *commit)
{
    PICC_CommitListElement *new_commit_list_element = (PICC_CommitListElement*)malloc(sizeof(PICC_CommitListElement));
    new_commit_list_element->commit = commit;
    new_commit_list_element->next = NULL;
    return new_commit_list_element;
}

/**
 * Function that add the selected element at the end of the CommitList
 * @param the PICC_CommitList in which to add a PICC_Commit
 * @param the PICC_Commit to add
 */
void PICC_commit_list_add(PICC_CommitList *commit_list, PICC_Commit *commit)
{
    PICC_CommitListElement *new_commit_list_element = PICC_create_commit_list_element(commit);
    commit_list->tail->next = new_commit_list_element;
    commit_list->tail = new_commit_list_element;
    commit_list->size++;
}

/**
 * Function that removes the first element from the commitlist and returns it
 * @param the PICC_CommitList in which to fetch
 * @return the PICC_Commit to retrieve
 */
PICC_Commit *PICC_commit_list_fetch(PICC_CommitList *commit_list)
{
    PICC_CommitListElement *commit_list_element = commit_list->head;
    commit_list->head = commit_list_element->next;
    commit_list->size--;
    commit_list_element->next = NULL;
    return commit_list_element->commit;
}


