/**
 * @file channel.c
 * Channels.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Dany SIRIPHOL
 * @author Joel HING
 * @author Mickaël MENU
 */

#include <stdio.h>
#include <value.h>
#include <channel_repr.h>
#include <commit_repr.h>
#include <knownset_repr.h>
#include <tools.h>
#include <error.h>

/**
 * Creates a channel which contains 10 commitments.
 *
 * @post channel->incommits->size == DEFAULT_CHANNEL_COMMIT_SIZE
 * @post channel->outcommits->size == DEFAULT_CHANNEL_COMMIT_SIZE
 * @return Created channel
 */
PICC_Channel *PICC_create_channel()
{
    PICC_Channel *channel = PICC_create_channel_cn(0, 0);

     #ifdef CONTRACT_POST_INV
        // inv
        PICC_Channel_inv(channel);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(channel->incommits->size == 0);
        ASSERT(channel->outcommits->size == 0);
    #endif

    return channel;

}

/**
 * Creates a channel which contains <intcommit_size> in commitments and <outcommit_size> out commitments.
 *
 * @pre incommit_size >= 0
 * @pre outcommit_size >= 0
 *
 * @post channel->incommits->size == incommit_size
 * @post channel->outcommits->size == outcommit_size
 * @return Created channel
 */
PICC_Channel *PICC_create_channel_cn(int incommit_size, int outcommit_size)
{
    #ifdef CONTRACT_PRE
        // pre
        ASSERT(incommit_size >= 0);
        ASSERT(outcommit_size >= 0);
    #endif

    ALLOC_ERROR(error);
    PICC_ALLOC(channel, PICC_Channel, &error) {
        channel->global_rc = 1;
        channel->lock = PICC_create_lock(&error);
	channel->reclaim = (PICC_Reclaimer) PICC_reclaim_channel;
        channel->incommits = PICC_create_commit_list(&error);
        channel->incommits->size = incommit_size;
        channel->outcommits = PICC_create_commit_list(&error);
        channel->outcommits->size = outcommit_size;
        if (channel->incommits == NULL || channel->outcommits == NULL) {
            NEW_ERROR(&error, ERR_OUT_OF_MEMORY);
            free(channel);
            channel = NULL;
        }
    }
    if (HAS_ERROR(error))
    {
        CRASH(&error);
    }
    #ifdef CONTRACT_POST_INV
        // inv
        PICC_Channel_inv(channel);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(channel->incommits->size == incommit_size );
        ASSERT(channel->outcommits->size == outcommit_size );
    #endif

    return channel;
}

/**
 * Reclaims the given channel.
 *
 * @param channel Channel to reclaim
 */
void PICC_reclaim_channel(PICC_Channel *channel, PICC_Error *error)
{
    free(channel->incommits);
    free(channel->outcommits);
    free(channel);
}

/**
 * Releases all the given channels.
 *
 * @param chans Set of channels to release
 * @param nb_chans
 */
void PICC_release_all_channels(PICC_KnownSet *chans)
{
    // /!\ will release all the managed value (for now channel and string)
    PICC_KnownValue *val;
    PICC_KNOWNSET_FOREACH(chans, val){
        RELEASE_CHANNEL(val->handle);
    }
}

/**
 * Checks channel invariant.
 *
 * @inv incommits != NULL && outcommits != NULL && global_rc > 0
 * @inv cont_pc > 0
 */
void PICC_Channel_inv(PICC_Channel *channel)
{
    ASSERT(channel->incommits != NULL);
    ASSERT(channel->outcommits != NULL);
    ASSERT(channel->global_rc > 0 );
}
