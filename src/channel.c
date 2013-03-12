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
 * Creates a new Knowns set.
 *
 * @pre size > 0
 *
 * @post knowns_set->size == size
 *
 * @param error Error stack
 * @return Created knowns set
 */
/* PICC_KnownsSet *PICC_create_knowns_set(int size, PICC_Error *error) */
/* { */
/*     #ifdef CONTRACT_PRE */
/*         // pre */
/*         ASSERT(size> 0); */
/*     #endif */

/*     PICC_ALLOC(knowns_set, PICC_KnownsSet, error) { */
/*         knowns_set->knowns = malloc(sizeof(PICC_Knowns) * size); */
/*         if (knowns_set->knowns == NULL) { */
/*             NEW_ERROR(error, ERR_OUT_OF_MEMORY); */
/*             free(knowns_set); */
/*             knowns_set = NULL; */
/*         } else { */
/*             knowns_set->size = size; */
/*         } */
/*     } */

/*     #ifdef CONTRACT_POST */
/*         // post */
/*         ASSERT(knowns_set->size == size); */
/*     #endif */
/*     return knowns_set; */
/* } */

/**
 * Increments the global reference count of a channel (the number of commitment referencing the channel).
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
 * Decrements the global reference count of a channel (the number of commitment referencing the channel) .
 *
 * @pre channel != NULL
 *
 * @post channel->global_rc = channel->global_rc@pre - 1
 *
 * @param Channel to update
 */
void PICC_channel_dec_ref_count(PICC_Channel **channel)
{
    #ifdef CONTRACT_PRE_INV
        //inv
        PICC_Channel_inv(*channel);
    #endif

    #ifdef CONTRACT_PRE
        //pre
        ASSERT(*channel != NULL );
    #endif

    #ifdef CONTRACT_POST
        // capture
        int global_rc_at_pre = (*channel)->global_rc;
    #endif

    LOCK_CHANNEL(*channel);
    (*channel)->global_rc--;
    RELEASE_CHANNEL(*channel);

    if ((*channel)->global_rc == 0) {
        ALLOC_ERROR(reclaim_error);
        PICC_reclaim_channel(*channel, &reclaim_error);
        *channel = NULL;
        if (HAS_ERROR(reclaim_error))
            CRASH(&reclaim_error);
    }

    #ifdef CONTRACT_POST_INV
        if (*channel != NULL)
            PICC_Channel_inv(*channel);
    #endif

    #ifdef CONTRACT_POST
        if(global_rc_at_pre > 1)
            ASSERT((*channel)->global_rc == global_rc_at_pre - 1 );
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


bool PICC_known_set_add_channel(PICC_KnownSet *s, PICC_Channel *c){
    return PICC_known_set_add(s, (GEN_VALUE *) PICC_create_channel_value(c));
}

/**
 * Releases all the given channels.
 *
 * @param chans Set of channels to release
 * @param nb_chans
 */
void PICC_release_all_channels(PICC_KnownSet *chans)
{
    PICC_KNOWNSET_FOREACH(PICC_Value, chv, chans, it);
        RELEASE_CHANNEL(PICC_channel_of_channel_value(chv));
    END_KNOWNSET_FOREACH;
}

void PICC_knowns_set_forget_to_unknown(PICC_KnownSet *ks, PICC_Channel *c) {
    PICC_knowns_set_forget_to_unknown_gen(ks, PICC_create_channel_value(c));
}

bool PICC_knowns_register(PICC_KnownSet *ks, PICC_Channel *c) {
    return PICC_knowns_register_gen(ks, PICC_create_channel_value(c));
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
