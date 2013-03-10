/**
 * @file gc.c
 * Second generation of garbage collector
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */


#include <gc_repr.h>
#include <queue_repr.h>
#include <concurrent.h>
#include <channel.h>
#include <pi_thread_repr.h>
#include <commit_repr.h>

bool PICC_GC2(PICC_SchedPool* sched)
{
    PICC_PiThread* clique[1000];
    int clique_size = 0;
    PICC_PiThread* candidate = PICC_wait_queue_pop_old(sched->wait);

    if(!(PICC_try_acquire(candidate->lock)))
    {
        PICC_wait_queue_push(sched->wait, candidate);
        return false;
    }

    PICC_PiThread* candidates[1000];
    PICC_KnownSet* chans = PICC_create_empty_known_set();
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
			if(PICC_known_set_add(chans, PICC_create_channel_value(chan))){
				if(!(PICC_try_acquire(chan->lock))){
					goto abandon_gc;
				}
			} else {
				continue;
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
    //while(candidates[i]->nb_slaves > 0); -- Maxence version, dunno WTF he is trying to do
    while(candidates_size > 0);

    for(int i = 0; i < clique_size; i++){
		// reclaim clique[i]	-- TODO
	}

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

	// releaseAllChannel -- TODO
	return false;

}