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

#include <channel.h>
#include <tools.h>

#define LOCK_CHANNEL(c) \
    PICC_acquire(&(c->lock));

#define RELEASE_CHANNEL(c) \
    PICC_release(&(c->lock), NULL);

/**
 * Creates a channel which contains 10 commitments.
 *
 * @param error Error stack
 * @return Created channel
 */
PICC_Channel *PICC_create_channel(PICC_Error *error)
{
    return PICC_create_channel_cn(10, error);
}

/**
 * Creates a channel which contains <commit_size> commitments.
 *
 * @param error Error stack
 * @return Created channel
 */
PICC_Channel *PICC_create_channel_cn(int commit_size, PICC_Error *error)
{
    PICC_ALLOC(channel, PICC_Channel, error) {
        channel->global_rc = 1;
        channel->incommits = malloc(sizeof(PICC_Commit) * commit_size);
        channel->outcommits = malloc(sizeof(PICC_Commit) * commit_size);
        if (channel->incommits == NULL || channel->outcommits == NULL) {
            NEW_ERROR(error, ERR_OUT_OF_MEMORY);
            free(channel);
            channel = NULL;
        }
    }
    return channel;
}

/**
 * Creates a new Knowns structure.
 *
 * @param error Error stack
 * @return Created knowns structure
 */
PICC_Knowns *PICC_create_knowns(PICC_Error *error)
{
    PICC_ALLOC(knowns, PICC_Knowns, error) {
        knowns->channel = NULL;
        knowns->state = PICC_UNKNOWN;
    }
    return knowns;
}

/**
 * Creates a new Knowns set.
 *
 * @param error Error stack
 * @return Created knowns set
 */
PICC_KnownsSet *PICC_create_knowns_set(int length, PICC_Error *error)
{
    PICC_ALLOC(knowns_set, PICC_KnownsSet, error) {
        knowns_set->knowns = malloc(sizeof(PICC_Knowns) * length);
        if (knowns_set->knowns == NULL) {
            NEW_ERROR(error, ERR_OUT_OF_MEMORY);
            free(knowns_set);
            knowns_set = NULL;
        } else {
            knowns_set->length = length;
        }
    }
    return knowns_set;
}

/**
 * Increments the global reference count of a channel.
 *
 * @param Channel to update
 */
void PICC_channel_incr_ref_count(PICC_Channel *channel , PICC_Error *error)
{
    LOCK_CHANNEL(channel);
    channel->global_rc++;
    RELEASE_CHANNEL(channel);
}

/**
 * Decrements the global reference count of a channel.
 *
 * @param Channel to update
 */
void PICC_channel_dec_ref_count(PICC_Channel *channel , PICC_Error *error)
{
    LOCK_CHANNEL(channel);
    channel->global_rc--;
    RELEASE_CHANNEL(channel);

    if (channel->global_rc == 0) {
        PICC_reclaim_channel(channel, error);
    }
}

/**
 * Reclaims the given channel.
 *
 * @param channel Channel to reclaim
 */
void PICC_reclaim_channel(PICC_Channel *channel, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
}

/**
 * Returns a subset of all KNOWN-STATE in a knows set.
 *
 * @param ks Knowns set
 * @param error Error stack
 * @return Subset of all known state in the given set
 */
PICC_KnownsSet *PICC_knowns_set_knows(PICC_KnownsSet *ks, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return NULL;
}

/**
 * Returns a subset of all FORGET-STATE in a knowns set.
 *
 * @param ks Knowns set
 * @param error Error stack
 * @return Subset of all forget state in the given set.
 */
PICC_KnownsSet *PICC_knowns_set_forget(PICC_KnownsSet *ks, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return NULL;
}

/**
 * Adds a channel to a knowns set.
 *
 * Looks for a channel in a PICC_KnownsSet
 * - if the channel is in the PICC_KnownsSet in KNOWN-STATE, it returns false
 * - if the channel is in the PICC_KnownsSet in FORGET-STATE, it switches it to KNOWN then  returns false
 * - else it add the channel in the PICC_KnownsSet (KNOWS-STATE) then returns true
 *
 * @param ks Knowns set
 * @param ch Channel to add
 * @param error Error stack
 * @return Whether the channel has been added
 */
bool PICC_knowns_register(PICC_KnownsSet *ks, PICC_Channel *ch, PICC_Error *error)
{
    NEW_ERROR(error, ERR_NOT_IMPLEMENTED);
    return false;
}
