/**
 * @file channel_repr.h
 * Channels.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Dany SIRIPHOL
 * @author Joel HING
 */

#ifndef CHANNEL_REPR_H
#define CHANNEL_REPR_H

#include <stdbool.h>
#include <commit.h>
#include <channel.h>
#include <concurrent.h>
#include <error.h>

#define DEFAULT_CHANNEL_COMMIT_SIZE 10

#define LOCK_CHANNEL(c) \
    PICC_acquire(&(c->lock));

#define RELEASE_CHANNEL(c) \
    PICC_release(&(c->lock));



/**
 * The type of the pi-thread channels
 */
struct _PICC_Channel {
    /**@{*/
    PICC_CommitList* incommits; /**< The input commits list */
    PICC_CommitList* outcommits; /**< The output commits list */
    int global_rc; /** The number of commitments to that reference

                    this channel (TODO see spec)*/
    PICC_Lock lock; /** This channel lock to protect from concurrent
                        accesses*/
    /**@}*/
};

/**
 * The known state of a channel
 */
enum _PICC_KnowsState {
    PICC_UNKNOWN, /**< TODO see spec */
    PICC_KNOWN, /**< TODO see spec */
    PICC_FORGET /**< TODO see spec */
};

/**
 * The type of a channel with a known state
 */
/* struct _PICC_Knowns { */
/*     /\**@{*\/ */
/*     PICC_Channel *channel; /\**< a reference to the tracked channel *\/ */
/*     PICC_KnownsState state; /\**< the known state *\/ */
/*     /\**@}*\/ */
/* }; */

/* /\** */
/*  * The set of known channels */
/*  *\/ */
/* struct _PICC_KnownsSet { */
/*     /\**@{*\/ */
/*     PICC_Knowns **knowns; /\** A pointer to an array of knowns. */
/*                             Contains all the data *\/ */
/*     int size; /\**< The size of the set *\/ */
/*     /\**@}*\/ */
/* }; */

extern PICC_Knowns *PICC_create_knowns(PICC_Channel *channel, PICC_Error *error);
//extern PICC_KnownsSet *PICC_create_knowns_set(int length, PICC_Error *error);
extern void PICC_reclaim_channel(PICC_Channel *channel, PICC_Error *error);
extern void PICC_free_channel(PICC_Channel *channel);

extern void PICC_Channel_inv(PICC_Channel *channel);
//extern void PICC_KnownsSet_inv(PICC_KnownsSet *set);
extern void PICC_Knowns_inv(PICC_Knowns *knowns);

#endif
