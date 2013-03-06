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
#include <concurrent_repr.h>

bool PICC_GC2(PICC_SchedPool* sched)
{
  PICC_PiThread* clique = malloc(sizeof(PICC_PiThread) * sched->nb_slaves);
  PICC_PiThread* candidate = malloc(sizeof(PICC_PiThread));
  candidate = PICC_wait_queue_pop_old(sched->wait);

  if(!PICC_try_acquire(candidate->lock))
    {
      PICC_wait_queue_push(sched->wait, candidate);
      return false;
    }

  PICC_PiThread** candidates = malloc(sizeof(PICC_PiThread*) * sched->nb_slaves);
  PICC_Channel* chans = malloc(sizeof(PICC_Channel) * candidate->knowns->size);
  candidates[0] = candidate;

  do
    {
      
    } while(candidates->nb_slaves > 0);
}
