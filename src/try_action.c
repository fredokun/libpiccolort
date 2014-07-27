
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
#include <knownset_repr.h>
#include <try_action.h>

/**
 * Utilitary function for inserting a channel in the array of acquired channel
 * (cf. compilation of choice)
 * @param new_chan the channel to insert/acquire.
 * @param chans the array of acquired channels
 * @param nbchans the number of acquired channels, writeable.
 * @return true if the channel has been added, otherwise false.
 **/
/*static bool chan_array_add(PICC_Channel* new_chan, PICC_Channel* chans[], int * nbchans) {
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
}*/

PICC_Channel** PICC_create_empty_channel_array(int maxsize)
{
  PICC_Channel** array = (PICC_Channel**)(malloc(maxsize * sizeof(PICC_Channel*)));
  return array;
}

bool PICC_channel_acquire_and_register(PICC_PiThread *pt, PICC_Channel *newchan, PICC_Channel **chans, int *nb_chans)
{
  int i;
  for (i = 0 ; i < *nb_chans ; i++) {
    if (chans[i] == newchan) {
      return true;
    }
  }
  if (!(PICC_process_acquire_channel(pt, newchan))) {
    return false;
  }
  chans[*nb_chans] = newchan;
  *nb_chans += 1;
  return true;
}

PICC_Commit *PICC_try_output_action(PICC_Channel *out_chan, PICC_TryResult *try_result)
{
  if(out_chan->global_rc == 1) {
    *try_result = PICC_TRY_DISABLED;
    return NULL;
  }

  PICC_Commit *commit = NULL;
  PICC_CommitStatus ok = PICC_CANNOT_ACQUIRE;

  do {
    // search for a valid commitment
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
        PICC_low_level_yield();
      }
    } while(ok == PICC_CANNOT_ACQUIRE);

    // here we have either a valid commitment
    // and an awoken thread, or an invalid commitment
    // that e just removed (lazy deletion)

    if(ok == PICC_VALID_COMMIT) {
      // ok, everything's fine a synchronization will occur
      *try_result = PICC_TRY_ENABLED;
      return commit;
    }
  } while (!(PICC_commit_list_is_empty(out_chan->incommits)));

  CRASH_NEW_ERROR(ERR_DEAD_CODE_REACHED);
  return NULL;
}

PICC_Commit *PICC_try_input_action(PICC_Channel *in_chan, PICC_TryResult *try_result)
{
  if (in_chan->global_rc == 1) {
    *try_result = PICC_TRY_DISABLED;
    return NULL;
  }

  PICC_Commit *commit = NULL;
  PICC_CommitStatus ok = PICC_CANNOT_ACQUIRE;

  do {
    commit = PICC_fetch_output_commitment(in_chan);
    if (commit == NULL) {
      *try_result = PICC_TRY_COMMIT;
      return NULL;
    }

    ok = PICC_can_awake(commit->thread, commit);

    do {
      if (ok == PICC_CANNOT_ACQUIRE) {
        PICC_low_level_yield();
      }
    } while (ok == PICC_CANNOT_ACQUIRE);

    if (ok == PICC_VALID_COMMIT) {
      *try_result = PICC_TRY_ENABLED;
      return commit;
    }
  } while (!(PICC_commit_list_is_empty(in_chan->outcommits)));

  CRASH_NEW_ERROR(ERR_DEAD_CODE_REACHED);
  return NULL;
}
