
/**
 * @file try_action.c
 * File that contains definitions of the prototypes specified in runtime.h
 *
 * This project is released under MIT License.
 *
 * @author Frederic Peschanski
 */

#include <stdbool.h>

#include <error.h>
#include <pi_thread_repr.h>
#include <channel.h>
#include <commit_repr.h>
#include <value.h>

#include <try_action.h>

/**
 * Utilitary function for inserting a channel in the array of acquired channel
 * (cf. compilation of choice)
 * @param new_chan the channel to insert/acquire.
 * @param chans the array of acquired channels
 * @param nbchans the number of acquired channels, writeable.
 * @return true if the channel has been added, otherwise false.
 **/
static bool chan_array_add(PICC_Channel* new_chan, PICC_Channel* chans[], int * nbchans) {
  int i = 0;
  while(i < *nbchans) {
    if(chans[i] == new_chan) {
      return false;
    }
    i += 1;
  }
  chans[*nbchans] = new_chan;
  *nbchans = (*nbchans) + 1;
  return true;
}
    

/**
 * The runtime support for output actions.
 *
 * @param pt the PiThread structure of the thread trying to output
 * @param chan_ref the environment index of the output channel
 * @param chans the acquired channels  (cf. compilation of output try)
 * @param nbchans the number of acquired channels 
 * @param try_result an output variable for the result of the try (ENABLED if output can be performed, DISABLED if not, and COMMIT if a commiment must be recorded).
 * @return the matching input commitment, if any or NULL otherwise.
 */
PICC_Commit * PICC_try_output_action(PICC_PiThread *pt, int chan_ref, PICC_Channel* chans[], int nbchans, PICC_TryResult * try_result) {
  PICC_Channel* out_chan = PICC_channel_of_channel_value(&(pt->env[chan_ref]));

  // acquire the channel, if required
  if (chan_array_add(out_chan, chans, &nbchans)) {
    LOCK_CHANNEL(out_chan);
  }
  
  if(out_chan->global_rc == 1) {
    // if global reference count is one, then pt is the only
    // thread knowing the channel, hence the try is DISABLED.
    *try_result = PICC_TRY_DISABLED;
    return NULL;
  }
  

  PICC_Commit * commit = NULL;
  PICC_CommitStatus ok = PICC_CANNOT_ACQUIRE;
  
  do {
    // search for a valid input commitment
    commit = PICC_fetch_input_commitment(out_chan);
    if(commit == NULL) {
      // if no valid or invalid commitment left, need to 
      // make an output commitment
      *try_result = PICC_TRY_COMMIT;
      return NULL;
    }
  
    // here, we have a candidate input commitment
  
    // try to awake the commiting thread
    do {
      ok = PICC_can_awake(commit->thread, commit);
      
      if(ok == PICC_CANNOT_ACQUIRE) {
        // to avoid contention on thread awaking
        PICC_low_level_yield();
      }
      
    } while(ok == PICC_CANNOT_ACQUIRE);

    // here we have either a valid commitment
    // an an awoken thread, or an invalid commitment
    // that we just removed  (lazy deletion).
    
    if (ok == PICC_VALID_COMMIT) {
      // ok, everything's fine a synchronization will occur
      *try_result = PICC_TRY_ENABLED;
      return commit; // we return the valid commitment
    }

    // otherwise it's invalid and deleted, and we will
    // try to find a further commitment
  } while (!(PICC_commit_list_is_empty(out_chan->incommits)));

  // TODO:  raise a "dead code reached" error
  CRASH_NEW_ERROR(ERR_DEAD_CODE_REACHED);

  return NULL;

}


