/**
 * @file entry.c
 * File that contains the entry point of the runtime library
 *
 * This project is released under MIT License.
 *
 * @author Sergiu Tiganu
 * @author Maxence WO
 * @author Dany Siriphol
 * @author Mickaël MENU
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

#include <runtime.h>
#include <error.h>
#include <entry.h>

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
	PICC_SchedPool sched_pool;
	PICC_PiThread *init_thread;
	PICC_Args * args = (PICC_Args *)malloc(sizeof(PICC_Args));
	args->error = error;
	args->sched_pool = &sched_pool;
	void* function = PICC_sched_pool_slave;
	int i;
	pthread_t *threads;

	int status;


	sched_pool = PICC_create_sched_pool();

	threads = (pthread_t *)malloc(nb_core_threads * sizeof(pthread_t));
	if (threads == NULL)
	{
		NEW_ERROR(&error, ERR_OUT_OF_MEMORY);
		CRASH(&error);
	}

	sched_pool.running = true;


	for (i = 0; i < nb_core_threads; ++i)
	{
		status = pthread_create(&threads[i], NULL,
			function, args);
		if (status)
		{
			NEW_ERROR(&error, ERR_THREAD_CREATE);
			CRASH(&error);
		}
		++(sched_pool.nb_slaves);
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

	PICC_ready_queue_push(sched_pool.ready, init_thread, &error);
	if (HAS_ERROR(error))
		CRASH(&error);

	PICC_sched_pool_master(sched_pool, 2, 2, 2);
}
