
/**
 * @file try_action.c
 * File that contains definitions of the prototypes specified in runtime.h
 *
 * This project is released under MIT License.
 *
 * @author Frederic Peschanski
 */

#include <pi_thread.h>

#include <try_action.h>

/**
 * The runtime support for output actions.
 *
 * @param pt the PiThread structure of the thread trying to output
 * @param chan_ref the environment index of the output channel
 * @param chans the acquired channels  (cf. compilation of output try) 
 * @param try_result an output variable for the result of the try (ENABLED if output can be performed, DISABLED if not, and COMMIT if a commiment must be recorded).
 * @return the matching input commitment, if any or NULL otherwise.
 */
PICC_InCommit * PICC_try_output_action(PICC_PiThread *pt, int chan_ref, PICC_ChanSet *chans, PICC_TryStatus * try_result) {
  

}


