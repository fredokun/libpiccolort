/**
 * @file channel.h
 * Channels.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Dany SIRIPHOL
 * @author Joel HING
 */

#ifndef CHANNEL_H
#define CHANNEL_H

#include <stdbool.h>
#include <commit.h>
#include <sync.h>
#include <error.h>

#define DEFAULT_CHANNEL_COMMIT_SIZE 10


#define LOCK_CHANNEL(c) \
    PICC_acquire(&(c->lock));

#define RELEASE_CHANNEL(c) \
    PICC_release(&(c->lock));



/**
 * The type of the pi-thread channels
 */
typedef struct _PICC_Channel {
    /**@{*/
    struct _PICC_CommitList* incommits; /**< The input commits list */
    struct _PICC_CommitList* outcommits; /**< The output commits list */
    int global_rc; /** The number of commitments to that reference

                    this channel (TODO see spec)*/
    PICC_Mutex lock; /** This channel lock to protect from concurrent
                        accesses*/
    /**@}*/
} PICC_Channel;

/**
 * The known state of a channel
 */
typedef enum _PICC_KnowsState {
    PICC_UNKNOWN, /**< TODO see spec */
    PICC_KNOWN, /**< TODO see spec */
    PICC_FORGET /**< TODO see spec */
} PICC_KnownsState;

/**
 * The type of a channel with a known state
 */
typedef struct _PICC_Knowns {
    /**@{*/
    PICC_Channel *channel; /**< a reference to the tracked channel */
    PICC_KnownsState state; /**< the known state */
    /**@}*/
} PICC_Knowns;

/**
 * The set of known channels
 */
typedef struct _PICC_KnownsSet {
    /**@{*/
    PICC_Knowns **knowns; /** A pointer to an array of knowns.
                            Contains all the data */
    int length; /**< The size of the set */
    /**@}*/
} PICC_KnownsSet;

extern PICC_Channel *PICC_create_channel();
extern PICC_Channel *PICC_create_channel_cn(int incommit_size,int outcommit_size);
extern PICC_Knowns *PICC_create_knowns(PICC_Error *error);
extern PICC_KnownsSet *PICC_create_knowns_set(int length, PICC_Error *error);
extern void PICC_channel_incr_ref_count(PICC_Channel *ch);
extern void PICC_channel_dec_ref_count(PICC_Channel *ch);
extern void PICC_reclaim_channel(PICC_Channel *channel, PICC_Error *error);
extern PICC_KnownsSet *PICC_knowns_set_knows(PICC_KnownsSet *ks);
extern PICC_KnownsSet *PICC_knowns_set_forget(PICC_KnownsSet *ks);
extern void PICC_knowns_set_forget_to_unknown(PICC_KnownsSet *ks, PICC_Channel *ch);
extern void PICC_knowns_set_forget_all(PICC_KnownsSet *ks);
extern bool PICC_knowns_register(PICC_KnownsSet *ks, PICC_Channel *ch);
extern void PICC_release_all_channels(PICC_Channel **chans, int nb_chans);

extern void PICC_free_channel(PICC_Channel *channel);

extern void PICC_all_channel_test();

#endif
