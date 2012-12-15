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

typedef struct _PICC_Channel {
    struct _PICC_Commit* incommits;
    struct _PICC_Commit* outcommits;
    int global_rc;
    PICC_Mutex lock;
} PICC_Channel;

typedef enum _PICC_KnowsState {
    PICC_UNKNOWN,
    PICC_KNOWN,
    PICC_FORGET
} PICC_KnownsState;

typedef struct _PICC_Knowns {
    PICC_Channel *channel;
    PICC_KnownsState state;
} PICC_Knowns;

typedef struct _PICC_KnownsSet {
    PICC_Knowns **knowns;
    int length;
} PICC_KnownsSet;

extern PICC_Channel *PICC_create_channel(PICC_Error *error);
extern PICC_Channel *PICC_create_channel_cn(int commit_size, PICC_Error *error);
extern PICC_Knowns *PICC_create_knowns(PICC_Error *error);
extern PICC_KnownsSet *PICC_create_knowns_set(int length, PICC_Error *error);
extern void PICC_channel_incr_ref_count(PICC_Channel *ch, PICC_Error *error);
extern void PICC_channel_dec_ref_count(PICC_Channel *ch, PICC_Error *error);
extern void PICC_reclaim_channel(PICC_Channel *channel, PICC_Error *error);
extern PICC_KnownsSet *PICC_knowns_set_knows(PICC_KnownsSet *ks, PICC_Error *error);
extern PICC_KnownsSet *PICC_knowns_set_forget(PICC_KnownsSet *ks, PICC_Error *error);
extern bool PICC_knowns_register(PICC_KnownsSet *ks, PICC_Channel *ch, PICC_Error *error);

#endif
