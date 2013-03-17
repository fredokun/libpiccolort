/**
 * @file gc.c
 * Second generation of garbage collector
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */


#include <gc_repr.h>
#include <knownset.h>
#include <queue_repr.h>
#include <channel.h>
#include <pi_thread_repr.h>
#include <commit_repr.h>
#include <stdio.h>
#include <tools.h>
/**
 * Increments the global reference count of a managed value
 *
 * @pre h != NULL
 *
 * @post h->global_rc = h->global_rc@pre + 1
 *
 * @param Handle to update
 */
void PICC_handle_incr_ref_count(PICC_Handle *h)
{
    /* #ifdef CONTRACT_PRE_INV */
    /*     //inv */
    /*     PICC_Channel_inv(channel); */
    /* #endif */

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(h != NULL );
    #endif

    #ifdef CONTRACT_POST
        // capture
        int global_rc_at_pre = h->global_rc;
    #endif

    LOCK_HANDLE(h);
    h->global_rc++;
    RELEASE_HANDLE(h);

    /* #ifdef CONTRACT_POST_INV */
    /*     //inv */
    /*     PICC_Channel_inv(channel); */
    /* #endif */

    /* #ifdef CONTRACT_POST */
    /*     //post */
    /*     ASSERT(channel->global_rc == global_rc_at_pre + 1 );  */
    /*     // cannot be asserted ! as the lock is released we don't have any garanty */
    /* #endif */
}

/**
 * Decrements the global reference count of a managed value
 *
 * @pre h != NULL
 *
 * @post h->global_rc = h->global_rc@pre - 1
 *
 * @param Handle to update
 */
void PICC_handle_dec_ref_count(PICC_Handle **h)
{
    /* #ifdef CONTRACT_PRE_INV */
    /*     //inv */
    /*     PICC_Channel_inv(*channel); */
    /* #endif */

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(*h != NULL );
    #endif

    #ifdef CONTRACT_POST
        // capture
        int global_rc_at_pre = (*h)->global_rc;
    #endif

	// I think there is a problem here: what happens if global_rc is equal to one
	// and another thread was about to increment it but lost the race to acquire the lock ?
	//
	// moreover, for now the lock is alocated in the channel/handle with the previous
	// case in mind we cannot free it in the reclaim
	// I think a solution would be to have a pool of Lock witch contain the lock itself
	// and the value it's suposed to lock
	//
	// kind of:
	// struct{
	//   atomic_int cpt;
	//   void *content;
        // }
	//
	// in the function lock_alloc we would keep track of all the locks and we could
	// reuse the ones that have cpt == 0 && content == NULL
	//

    LOCK_HANDLE(*h);
    (*h)->global_rc--;
    
    if ((*h)->global_rc == 0) {
	RELEASE_HANDLE(*h);

	ALLOC_ERROR(reclaim_error);
        (*h)->reclaim(*h, &reclaim_error);
	*h = NULL;
        if (HAS_ERROR(reclaim_error))
            CRASH(&reclaim_error);
    }
    else{
	RELEASE_HANDLE(*h);
    }

    /* #ifdef CONTRACT_POST_INV */
    /*     if (*h != NULL) */
    /*         PICC_Channel_inv(*channel); */
    /* #endif */

    /* #ifdef CONTRACT_POST */
    /*     if(global_rc_at_pre > 1) */
    /*         ASSERT((*channel)->global_rc == global_rc_at_pre - 1 ); */
    //cannot be asserted see incr for the same comment
    /* #endif */
}

bool PICC_GC2(PICC_SchedPool* sched)
{
    PICC_PiThread* clique[1000];
    int clique_size = 0;
    PICC_PiThread* candidate = PICC_wait_queue_pop_old(sched->wait);
    
    printf("PICC_GC2 - GO !!\n");
    if(!(PICC_try_acquire(candidate->lock)))
    {
        PICC_wait_queue_push(sched->wait, candidate);
        return false;
    }

    PICC_PiThread* candidates[1000];
    PICC_KnownSet* chans = PICC_create_empty_knownset();
	candidates[0] = candidate;
    int candidates_size = 1;

    do
    {
		// arbitrary choice of a piThread, here the first piThread of candidates
	    candidate = candidates[0];

		PICC_Commit* commit = NULL;
		PICC_CommitListElement* commitEl = candidate->commits->head;
		while(commitEl){
			commit = commitEl->commit;
			PICC_Channel* chan = commit->channel;
			int refs = 1;

			PICC_knownset_add(chans, (PICC_KnownValue*)PICC_create_channel_value(chan));
			if (!(PICC_try_acquire(chan->lock))) {
				goto abandon_gc;
			}
			PICC_Commit *incommit = NULL;
			do{
				incommit = PICC_fetch_input_commitment(chan);
				if(PICC_is_valid_commit(incommit)){
					if(!(PICC_try_acquire(incommit->thread->lock))){
						PICC_wait_queue_push(sched->wait, incommit->thread);
						goto abandon_gc;
					}
					if(incommit->thread->status != PICC_STATUS_WAIT){
						goto abandon_gc;
					}
					PICC_wait_queue_fetch(sched->wait, incommit->thread);

					int can_add = 1;
					for(int i = 0; i < candidates_size; i++){
						if(candidates[i] == incommit->thread){
							can_add = 0;
							break;
						}
					}
					if(can_add){
						refs++;
						candidates[candidates_size] = incommit->thread;
						candidates_size++;
					}
				} else {
					PICC_commit_list_remove(chan->incommits, incommit);
				}

			} while(incommit);

			PICC_Commit *outcommit = NULL;
			do{
				outcommit = PICC_fetch_output_commitment(chan);
				if(PICC_is_valid_commit(outcommit)){
					if(outcommit->thread->status != PICC_STATUS_WAIT){
						goto abandon_gc;
					}
					// NOT SURE: PICC_wait_queue_pop in spec
					PICC_wait_queue_fetch(sched->wait, outcommit->thread);

					if(!(PICC_try_acquire(outcommit->thread->lock))){
						PICC_wait_queue_push(sched->wait, outcommit->thread);
						goto abandon_gc;
					}

					int can_add = 1;
					for(int i = 0; i < candidates_size; i++){
						if(candidates[i] == outcommit->thread){
							can_add = 0;
							break;
						}
					}
					if(can_add){
						refs++;
						candidates[candidates_size] = outcommit->thread;
						candidates_size++;
					}
				} else {
					PICC_commit_list_remove(chan->outcommits, outcommit);
				}

			} while(outcommit);

			if(refs < chan->global_rc){
				goto abandon_gc;
			}

			commitEl = commitEl->next;
		}

		int can_add = 1;
		for(int i = 0; i < clique_size; i++){
			if(clique[i] == candidate){
				can_add = 0;
				break;
			}
		}
		if(can_add){
			clique[clique_size] = candidate;
			clique_size++;
		}


    }
    while(candidates_size > 0);

    for(int i = 0; i < clique_size; i++){
        PICC_reclaim_pi_thread(clique[i]);
	}

    printf("GC don't release !!\n");
    return true;

    abandon_gc:
    for(int i = 0; i < clique_size; i++){
		PICC_wait_queue_push(sched->wait, clique[i]);
		PICC_release(clique[i]->lock);
	}

	for(int i = 0; i < candidates_size; i++){
		PICC_wait_queue_push(sched->wait, clique[i]);
		PICC_release(clique[i]->lock);
	}
	
	printf("GC release all !!\n");
	PICC_release_all_channels(chans); //-> released one by one in the loop
	return false;

}
