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

bool PICC_GC2(PICC_SchedPool* sched)
{
    PICC_PiThread* clique;
    PICC_PiThread* candidate = PICC_wait_queue_pop_old(sched->wait);
    
    // if(!(PICC_try_acquire(candidate->lock)))
    if(true)
    {
        PICC_wait_queue_push(sched->wait, candidate);
        return false;
    }
    
    //temp...
    PICC_PiThread* candidates[1000];
    PICC_Channel* chans;
    candidates[0] = candidate;
    int i = 0;
    
    do
    {
        
    } while(true);//while(candidates[i]->nb_slaves > 0);
    
    return true;
}
