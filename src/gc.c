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
        // int global_rc_at_pre = h->global_rc;
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
        // int global_rc_at_pre = (*h)->global_rc;
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
    int clique_max_size = PICC_init_clique_max_size;
    PICC_ALLOC_N_CRASH(clique, PICC_PiThread*, clique_max_size) {
        int clique_size = 0;
        PICC_PiThread* candidate = PICC_wait_queue_pop_old(sched->wait);
        if (candidate == NULL) {
            free(clique);
            return false;
        }
        
        if(!(PICC_try_acquire(candidate->lock)))
        {
            printf("1. GC pushed in wait queue: %p\n", candidate);
            PICC_wait_queue_push(sched->wait, candidate);
            free(clique);
            return false;
        }

        int candidates_max_size = PICC_init_candidates_max_size;
        PICC_ALLOC_N_CRASH(candidates, PICC_PiThread*, candidates_max_size) {
            PICC_KnownSet* chans = PICC_create_empty_knownset();
            candidates[0] = candidate;
            int candidates_size = candidate != NULL;

            while(candidates_size > 0)
            {
                // arbitrary choice of a piThread, here the first piThread of candidates
                candidate = candidates[0];
                --candidates_size;
                for (int i = 0; i < candidates_size; ++i) {
                    candidates[i] = candidates[i + 1];
                }        

                PICC_Commit* commit = NULL;
                PICC_CommitListElement* commitEl = candidate->commits->head;
                printf("Commits size %d\n", candidate->commits->size);
                while(commitEl){
                    commit = commitEl->commit;
                    if(PICC_is_valid_commit(commit)){
                        PICC_Channel* chan = commit->channel;
                        int refs = 1;
                        
                        if (!(PICC_try_acquire(chan->lock))) {
                            goto abandon_gc;
                        }
                        PICC_knownset_add(chans, (PICC_KnownValue*)PICC_create_channel_value(chan));
                        PICC_CommitListElement* incommitEl = chan->incommits->head;
                        PICC_Commit *incommit = NULL;
                        while(incommitEl) {
                            incommit = incommitEl->commit;
                            if(PICC_is_valid_commit(incommit)){
                                if (incommit->thread != candidate) {
                                    if(incommit->thread->status != PICC_STATUS_WAIT){
                                        goto abandon_gc;
                                    }                                    
    
                                    if(!(PICC_try_acquire(incommit->thread->lock))){                                        
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
                                        if (candidates_size >= candidates_max_size) {
                                            candidates_max_size *= 2;
                                            PICC_REALLOC_N_CRASH(candidates, PICC_PiThread*, candidates_max_size) { }
                                        }
                                        candidates[candidates_size] = incommit->thread;
                                        candidates_size++;
                                    }
                                }
                            } else {
                                PICC_commit_list_remove(chan->incommits, incommit);
                            }
                            incommitEl = incommitEl->next;
                        }

                        PICC_CommitListElement* outcommitEl = chan->outcommits->head;
                        PICC_Commit *outcommit = NULL;
                        while(outcommitEl){
                            outcommit = outcommitEl->commit;
                            if(PICC_is_valid_commit(outcommit)){
                                if (outcommit->thread != candidate) {
                                    if(outcommit->thread->status != PICC_STATUS_WAIT){
                                        goto abandon_gc;
                                    }                                    

                                    if(!(PICC_try_acquire(outcommit->thread->lock))){
                                        goto abandon_gc;
                                    }
                                    PICC_wait_queue_fetch(sched->wait, outcommit->thread);

                                    int can_add = 1;
                                    for(int i = 0; i < candidates_size; i++){
                                        if(candidates[i] == outcommit->thread){
                                            can_add = 0;
                                            break;
                                        }
                                    }
                                    if(can_add){
                                        refs++;
                                        if (candidates_size >= candidates_max_size) {
                                            candidates_max_size *= 2;
                                            PICC_REALLOC_N_CRASH(candidates, PICC_PiThread*, candidates_max_size) { }                                                
                                        }
                                        candidates[candidates_size] = outcommit->thread;
                                        candidates_size++;                                            
                                    }
                                }
                            } else {
                                PICC_commit_list_remove(chan->outcommits, outcommit);
                            }
                            outcommitEl = outcommitEl->next;
                        }

                        if(refs < chan->global_rc){
                            goto abandon_gc;
                        }
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
                    if (clique_size >= clique_max_size) {
                        clique_max_size *= 2;
                        PICC_REALLOC_N_CRASH(clique, PICC_PiThread*, clique_max_size) { }
                    }
                    clique[clique_size] = candidate;
                    clique_size++;
                }
            }

            for(int i = 0; i < clique_size; i++){
                printf("inserted in clique: %p\n", clique[i]);
                PICC_reclaim_pi_thread(clique[i]);
            }

            free(clique);
            free(candidates);
            printf("<GC clique found and freed>\n");
            return true;

            abandon_gc:
            for(int i = 0; i < clique_size; i++){
                printf("2. GC pushed in wait queue: %p\n", clique[i]);
                PICC_wait_queue_push(sched->wait, clique[i]);
                PICC_release(clique[i]->lock);
            }

            for(int i = 0; i < candidates_size; i++){
                printf("3. GC pushed in wait queue: %p\n", candidates[i]);
                PICC_wait_queue_push(sched->wait, candidates[i]);
                PICC_release(candidates[i]->lock);
            }
            printf("4. GC pushed in wait queue: %p\n", candidate);
            PICC_wait_queue_push(sched->wait, candidate);
            PICC_release(candidate->lock);

            printf("<GC no clique found, abandon>\n");
            PICC_release_all_channels(chans); //-> released one by one in the loop
            free(clique);
            free(candidates);
            return false;
        }
    }
}
