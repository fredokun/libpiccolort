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
#include <channel_repr.h>
#include <commit_repr.h>
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
 * Creates a new Knowns structure.
 *
 * @pre channel != NULL
 *
 * @post knows->state == PICC_UNKNOWN
 * @post knows->channel == channel
 *
 * @param error Error stack
 * @return Created knowns structure
 */
PICC_Knowns *PICC_create_knowns(PICC_Channel *channel, PICC_Error *error)
{
    #ifdef CONTRACT_PRE
        //pre
        ASSERT(channel != NULL );
    #endif

    PICC_ALLOC(knowns, PICC_Knowns, error) {
        knowns->channel = channel;
        knowns->state = PICC_UNKNOWN;
    }

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_Knowns_inv(knowns);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(knowns->state == PICC_UNKNOWN );
        ASSERT(knowns->channel == channel );
    #endif
    return knowns;
}

/**
 * Creates a new Knowns set.
 *
 * @pre size > 0
 *
 * @post knowns_set->size == size
 *
 * @param error Error stack
 * @return Created knowns set
 */
PICC_KnownsSet *PICC_create_knowns_set(int size, PICC_Error *error)
{
    #ifdef CONTRACT_PRE
        // pre
        ASSERT(size> 0);
    #endif

    PICC_ALLOC(knowns_set, PICC_KnownsSet, error) {
        knowns_set->knowns = malloc(sizeof(PICC_Knowns) * size);
        if (knowns_set->knowns == NULL) {
            NEW_ERROR(error, ERR_OUT_OF_MEMORY);
            free(knowns_set);
            knowns_set = NULL;
        } else {
            knowns_set->size = size;
        }
    }

    #ifdef CONTRACT_POST
        // post
        ASSERT(knowns_set->size == size);
    #endif
    return knowns_set;
}

/**
 * Increments the global reference count of a channel.
 *
 * @pre channel != NULL
 *
 * @post channel->global_rc = channel->global_rc@pre + 1
 *
 * @param Channel to update
 */
void PICC_channel_incr_ref_count(PICC_Channel *channel)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_Channel_inv(channel);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(channel != NULL );
    #endif

    #ifdef CONTRACT_POST
        // capture
        int global_rc_at_pre = channel->global_rc;
    #endif

    LOCK_CHANNEL(channel);
    channel->global_rc++;
    RELEASE_CHANNEL(channel);

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_Channel_inv(channel);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(channel->global_rc == global_rc_at_pre + 1 );
    #endif
}

/**
 * Decrements the global reference count of a channel.
 *
 * @pre channel != NULL
 *
 * @post channel->global_rc = channel->global_rc@pre - 1
 *
 * @param Channel to update
 */
void PICC_channel_dec_ref_count(PICC_Channel *channel)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_Channel_inv(channel);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(channel != NULL );
    #endif

    #ifdef CONTRACT_POST
        // capture
        int global_rc_at_pre = channel->global_rc;
    #endif

    LOCK_CHANNEL(channel);
    channel->global_rc--;
    RELEASE_CHANNEL(channel);

    if (channel->global_rc == 0) {
        ALLOC_ERROR(reclaim_error);
        PICC_reclaim_channel(channel, &reclaim_error);
        channel = NULL;
        if (HAS_ERROR(reclaim_error))
            CRASH(&reclaim_error);
    }

    #ifdef CONTRACT_POST_INV
        if (channel != NULL)
            PICC_Channel_inv(channel);
    #endif

    #ifdef CONTRACT_POST
        if(global_rc_at_pre > 1)
            ASSERT(channel->global_rc == global_rc_at_pre - 1 );
    #endif
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
 * search within a knownsSet the knowns with a specific state
 *
 * @pre ks != NULL
 *
 * @post each Knowns in result has the correct state
 * @post result set has the correct size
 *
 * @param state state wanted
 * @param ks knownsSet fetched
 */
PICC_KnownsSet *PICC_knowns_set_search(PICC_KnownsSet *ks, PICC_KnownsState state)
{

    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_KnownsSet_inv(ks);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
    #endif

    int count=0;
    int i;
    PICC_Knowns *known;
    for( i = 0 ; i < ks->size ; i++)
    {
        known = ks->knowns[i];
        if( known->state == state )
        {
            count ++;
        }
    }

    PICC_KnownsSet *result = PICC_create_knowns_set(count, NULL);
    count = 0;

    for( i = 0 ; i < ks->size ; i++)
    {
        known = ks->knowns[i];
        if( known->state == state )
        {
            result->knowns[count] = known;
            count++;
        }
    }

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_KnownsSet_inv(result);
        PICC_KnownsSet_inv(ks);
    #endif

    #ifdef CONTRACT_POST
        //post
        ASSERT(result != NULL);
        ASSERT(result->size == count);

        for(i=0;i<count;i++)
        {
            ASSERT(result->knowns[i]->state == state );
        }
    #endif

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
 * @pre ks != NULL
 * @pre ch != NULL

 * @param ks Knows set
 * @param ch Channel to switch state
 */
void PICC_knowns_set_forget_to_unknown(PICC_KnownsSet *ks, PICC_Channel *ch)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_KnownsSet_inv(ks);
        PICC_Channel_inv(ch);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
        ASSERT(ch != NULL);
    #endif

    int i;
    PICC_Knowns *known;
    for( i = 0 ; i < ks->size ; i++)
    {
        known = ks->knowns[i];
        if(known->channel == ch)
            known->state = PICC_UNKNOWN;

    }

    #ifdef CONTRACT_POST_INV
        //inv
        PICC_KnownsSet_inv(ks);
        PICC_Channel_inv(ch);
    #endif
}

/**
 * Switches all KNOWN state elements of a KnowsSet to FORGET state.
 *
 * @pre ks != NULL
 *
 * @post ks only have knowns values with PICC_FORGET state
 *
 * @param ks Knows set
 */
void PICC_knowns_set_forget_all(PICC_KnownsSet *ks)
{
    #ifdef CONTRACT_PRE_INV
         //inv
         PICC_KnownsSet_inv(ks);
    #endif

     #ifdef CONTRACT_PRE
         //pre
         ASSERT(ks != NULL);
    #endif

    int i;
    PICC_Knowns *known;
    for( i = 0 ; i < ks->size ; i++)
    {
        known = ks->knowns[i];
        known->state = PICC_FORGET;
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_KnownsSet_inv(ks);
    #endif

    #ifdef CONTRACT_POST
        // post
        for(i=0;i<ks->size;i++)
        {
            ASSERT(ks->knowns[i]->state == PICC_FORGET );
        }
    #endif
}

/**
 * Adds a channel to a knowns set.
 *
 * Looks for a channel in a PICC_KnownsSet
 * - if the channel is in the PICC_KnownsSet in KNOWN-STATE, it returns false
 * - if the channel is in the PICC_KnownsSet in FORGET-STATE, it switches it to KNOWN then  returns false
 * - else it add the channel in the PICC_KnownsSet (KNOWS-STATE) then returns true
 *
 * @pre ks != NULL
 *
 * @param ks Knowns set
 * @param ch Channel to add
 * @return Whether the channel has been added
 */
bool PICC_knowns_register(PICC_KnownsSet *ks, PICC_Channel *ch)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_KnownsSet_inv(ks);
        PICC_Channel_inv(ch);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(ks != NULL);
    #endif

    bool registered = true;
    int i;
    PICC_Knowns *known;
    for( i = 0 ; i < ks->size ; i++)
    {
        known = ks->knowns[i];
        if(known->channel == ch)
        {
            if(known->state == PICC_KNOWN)
            {
                registered = false;

            } else if(known->state == PICC_FORGET) {
                registered = false;
            }
        }
    }

    #ifdef CONTRACT_POST_INV
        // inv
        PICC_KnownsSet_inv(ks);
        PICC_Channel_inv(ch);
    #endif

    #ifdef CONTRACT_POST
        // post
        if (!registered) {
            ASSERT(known->state == PICC_KNOWN);
        }
    #endif

    return registered;
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

/**
 * Checks knowns invariant.
 *
 * @inv knowns->channel != NULL
 */
void PICC_Knowns_inv(PICC_Knowns *knowns)
{
    ASSERT(knowns->channel != NULL);
}

/**
 * Checks knownsSet invariant.
 *
 * @inv knownsSet->size > -1
 */
void PICC_KnownsSet_inv(PICC_KnownsSet *set)
{
    ASSERT(set->size > -1);
}
