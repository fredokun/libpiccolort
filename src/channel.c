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
#include <channel.h>
#include <tools.h>
#include <error.h>


/**
 * Creates a channel which contains 10 commitments.
 *
 * @return Created channel
 */
PICC_Channel *PICC_create_channel()
{
    return PICC_create_channel_cn(DEFAULT_CHANNEL_COMMIT_SIZE ,DEFAULT_CHANNEL_COMMIT_SIZE );
}

/**
 * Creates a channel which contains <commit_size> commitments.
 *
 * @return Created channel
 */
PICC_Channel *PICC_create_channel_cn(int incommit_size,int outcommit_size)
{
    ALLOC_ERROR(error);
    PICC_ALLOC(channel, PICC_Channel, &error) {
        channel->global_rc = 1;
        channel->incommits = malloc(sizeof(PICC_CommitList));
        channel->incommits->size = incommit_size;
        channel->outcommits = malloc(sizeof(PICC_CommitList));
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
    /*
    ASSERT(channel != NULL );
    ASSERT(channel->global_rc != 1);
    */

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
void PICC_channel_incr_ref_count(PICC_Channel *channel)
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
void PICC_channel_dec_ref_count(PICC_Channel *channel)
{
    LOCK_CHANNEL(channel);
    channel->global_rc--;
    RELEASE_CHANNEL(channel);

    if (channel->global_rc == 0) {
        ALLOC_ERROR(reclaim_error);
        PICC_reclaim_channel(channel, &reclaim_error);
        if (HAS_ERROR(reclaim_error))
            CRASH(&reclaim_error);
    }
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

PICC_KnownsSet *PICC_knowns_set_search(PICC_KnownsSet *ks, PICC_KnownsState state)
{
    int count=0;
    int i;
    PICC_Knowns *known;
    for( i = 0 ; i < ks->length ; i++)
    {
        known = ks->knowns[i];
        if( known->state == state )
        {
            count ++;
        }
    }
    PICC_KnownsSet *result = PICC_create_knowns_set(count, NULL);
    count = 0;

    for( i = 0 ; i < ks->length ; i++)
    {
        known = ks->knowns[i];
        if( known->state == state )
        {
            result->knowns[count] = known;
            count++;
        }
    }
    return result;
}

/**
 * Returns a subset of all KNOWN-STATE in a knows set.
 *
 * @param ks Knowns set
 * @return Subset of all known channel in the given set
 */
PICC_KnownsSet *PICC_knowns_set_knows(PICC_KnownsSet *ks)
{
    return PICC_knowns_set_search(ks, PICC_KNOWN);
}

/**
 * Returns a subset of all FORGET-STATE in a knowns set.
 *
 * @param ks Knowns set
 * @return Subset of all forget state in the given set.
 */
PICC_KnownsSet *PICC_knowns_set_forget(PICC_KnownsSet *ks)
{
    return PICC_knowns_set_search(ks, PICC_FORGET);
}

/**
 * Switches an element of a KnowsSet from the FORGET state to the
 * UNKNOWN state.
 *
 * @param ks Knows set
 * @param ch Channel to switch state
 */
void PICC_knowns_set_forget_to_unknown(PICC_KnownsSet *ks, PICC_Channel *ch)
{
    PICC_knowns_set_search(ks, PICC_UNKNOWN);
}

/**
 * Switches all KNOWN state elements of a KnowsSet to FORGET state.
 *
 * @param ks Knows set
 */
void PICC_knowns_set_forget_all(PICC_KnownsSet *ks)
{
   int i;
   PICC_Knowns *known;
   for( i = 0 ; i < ks->length ; i++)
   {
       known = ks->knowns[i];
       known->state = PICC_FORGET;
   }
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
 * @return Whether the channel has been added
 */
bool PICC_knowns_register(PICC_KnownsSet *ks, PICC_Channel *ch)
{
    int i;
    PICC_Knowns *known;
    for( i = 0 ; i < ks->length ; i++)
    {
        known = ks->knowns[i];
        if(known->channel == ch)
        {
            if(known->state == PICC_KNOWN)
            {
                return false;
            }
            else if(known->state == PICC_FORGET)
            {
                known->state = PICC_KNOWN;
                return false;
            }
        }
    }
    return true;
}

/**
 * Releases all the given channels.
 *
 * @param chans Set of channels to release
 * @param nb_chans
 */
void PICC_release_all_channels(PICC_Channel **chans, int nb_chans)
{
    int i;
    for (i = 0; i < nb_chans; i++)
        RELEASE_CHANNEL(chans[i]);
}
