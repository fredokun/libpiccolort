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

#include <gc.h>
#include <stdbool.h>
#include <commit.h>
#include <channel.h>
#include <concurrent.h>
#include <error.h>

#define DEFAULT_CHANNEL_COMMIT_SIZE 10

#define LOCK_CHANNEL(c) \
    PICC_acquire(((c)->lock));

#define RELEASE_CHANNEL(c) \
    PICC_release(((c)->lock));



/**
 * The type of the pi-thread channels
 */
struct _PICC_Channel 
{ //"implements PICC_Handle" cf gc_repr.h
    /**@{*/
    //global_rc and lock have to be first for PICC_channel to be "castable" as a PICC_KnownHandle
    int global_rc; /** The number of commitments to that reference

                    this channel (TODO see spec)*/
    PICC_Lock *lock; /** This channel lock to protect from concurrent
                        accesses*/
    PICC_Reclaimer reclaim;

    PICC_CommitList* incommits; /**< The input commits list */
    PICC_CommitList* outcommits; /**< The output commits list */
    /**@}*/
};


/* /\** */
/*  * The set of known channels */
/*  *\/ */
/* struct _PICC_KnownSet { */
/*     /\**@{*\/ */
/*     PICC_PiResource **knowns; /\** A pointer to an array of knowns. */
/*                             Contains all the data *\/ */
/*     int size; /\**< The size of the set *\/ */
/*     /\**@}*\/ */
/* }; */


//extern PICC_KnownSet *PICC_create_knowns_set(int length, PICC_Error *error);
extern void PICC_reclaim_channel(PICC_Channel *channel, PICC_Error *error);
extern void PICC_free_channel(PICC_Channel *channel);

extern void PICC_Channel_inv(PICC_Channel *channel);
//extern void PICC_KnownSet_inv(PICC_KnownSet *set);

#endif
