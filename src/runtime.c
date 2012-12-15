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
 * Creates and returns a commitment
 *
 * @return an input commitment
 */
PICC_Commit *PICC_create_commitment()
{
    PICC_Commit *new_commit = (PICC_Commit*)malloc(sizeof(PICC_Commit));
    new_commit->thread = malloc(sizeof(PICC_PiThread));
    new_commit->clock = malloc(sizeof(PICC_Clock));
    new_commit->channel = malloc(sizeof(PICC_Channel));

    return new_commit;
}

/**
 * Function that register an output PICC_Commit according to pithread and channel
 *
 * @param the PICC_PiThread used to create the output PICC_Commit
 * @param the PICC_Channel used to create the ourpur PICC_Commit
 * @param refvar the index of the var used to create the output PICC_Commit
 * @param cont_pc
 */
void PICC_register_out_commitment(PICC_PiThread *pi_thread, PICC_Channel *channel, PICC_EvalFunction *function, int cont_pc)
{
    PICC_OutCommit *out = (PICC_OutCommit *)malloc(sizeof(PICC_OutCommit));
    out->eval_func = function;

    PICC_Commit *out_commit = PICC_create_commitment();
    out_commit->content.out = out;
    out_commit->thread = pi_thread;
    out_commit->cont_pc = cont_pc;
    out_commit->type = PICC_OUT_COMMIT;
    out_commit->clock = pi_thread->clock;
    out_commit->clockval = pi_thread->clock->val;
    out_commit->channel = channel;
    PICC_commit_list_add(pi_thread->commits ,out_commit);
}

/**
 * Function that register an input PICC_Commit according to pithread and channel
 * @param the PICC_PiThread used to create the input PICC_Commit
 * @param the PICC_Channel used to create the input PICC_Commit
 * @param refvar the index of the var used to create the input PICC_Commit
 * @param cont_pc
 */
void PICC_register_in_commitment(PICC_PiThread *pi_thread, PICC_Channel *channel, int refvar, int cont_pc)
{
    PICC_InCommit *in = (PICC_InCommit *)malloc(sizeof(PICC_InCommit));
    in->refvar = refvar;

    PICC_Commit *in_commit = PICC_create_commitment();
    in_commit->content.in = in;
    in_commit->thread = pi_thread;
    in_commit->cont_pc = cont_pc;
    in_commit->type = PICC_IN_COMMIT;
    in_commit->clock = pi_thread->clock;
    in_commit->clockval = pi_thread->clock->val;
    in_commit->channel = channel;
    PICC_commit_list_add(pi_thread->commits ,in_commit);
}

/**
 * Function that verify if the PICC_Commit is valid
 * @param the PICC_Commit whose validity to check
 * @return true if the PICC_Commit is valid, false otherwise
 */
bool PICC_is_valid_commit(PICC_Commit *commit, PICC_Error *error)
{
    PICC_acquire_int(commit->thread->clock->val);
    if(commit->clock == commit->thread->clock)
        if(commit->clockval->value == commit->thread->clock->val->value)
        {
            PICC_release_int(commit->thread->clock->val, error);
            return true;
        }
    PICC_release_int(commit->thread->clock->val, error);
    return false;
}

/**
 * Function that fetch a PICC_Commit from a PICC_Channel
 * @param the PICC_Channel in which to fetch
 * @return the PICC_Commit fetched
 */
PICC_Commit PICC_fetch_commitment(PICC_Channel *channel)
{
    PICC_Commit current_commit;

    return current_commit;
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

/**
 * Increments by 1 the global reference count of a channel
 *
 * @param channel to update
 */
void PICC_channel_incr_ref_count(PICC_Channel channel , PICC_Error *error)
{
    PICC_acquire_bool(&channel.lock);
    channel.global_rc += 1;
    PICC_release_bool(&channel.lock, error);
}



void PICC_reclaim_channel( PICC_Channel channel )
{
    printf("Not implemented yet.\n");
    return;
}

/**
 * decrements by 1 the global reference count of a channel
 *
 * @param channel to update
 */
void PICC_channel_dec_ref_count( PICC_Channel channel , PICC_Error *error)
{
    PICC_acquire_bool(&channel.lock);
    channel.global_rc -= 1;
    PICC_release_bool(&channel.lock,error);

    if(channel.global_rc == 0 )
    {
        PICC_reclaim_channel(channel);
    }
}



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

/**
 * Function that returns a subset of all KNOWN-STATE in a knowsset
 * @param ks PICC_Knowsset
 * @return PICC_KnowsSet
 */
PICC_KnownsSet PICC_knows_set_knows(PICC_KnownsSet ks)
{
    printf("Not implemented yet.\n");
    return ks;
}

/**
 * Function that returns a subset of all FORGET-STATE in a Knowsset
 * @param ks PICC_Knowsset
 * @return PICC_KnowsSet
 */
PICC_KnownsSet PICC_knows_set_forget(PICC_KnownsSet ks)
{
    printf("Not implemented yet.\n");
    return ks;
}

/**
 * Function that looks for a channel in a PICC_KnownsSet
 * - if the channel is in the PICC_KnownsSet in KNOWN-STATE, it returns false
 * - if the channel is in the PICC_KnownsSet in FORGET-STATE, it switches it to KNOWN then  returns false
 * - else it add the channel in the PICC_KnownsSet (KNOWS-STATE) then returns true
 * @param ks PICC_Knowsset
 * @param ch PICC_Channel
 * @return b bool
 */
bool PICC_knows_register(PICC_KnownsSet ks, PICC_Channel ch)
{
    printf("Not implemented yet.\n");
    return false;
}



