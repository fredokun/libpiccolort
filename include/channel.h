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

/**
 * The type of the pi-thread channels
 */
typedef struct _PICC_Channel PICC_Channel;

/**
 * The known state of a channel
 */
typedef enum _PICC_KnowsState PICC_KnownsState;

/**
 * The type of a channel with a known state
 */
typedef struct _PICC_Knowns PICC_Knowns;

/**
 * The set of known channels
 */
typedef struct _PICC_KnownsSet PICC_KnownsSet;

extern PICC_Channel *PICC_create_channel();
extern PICC_Channel *PICC_create_channel_cn(int incommit_size,int outcommit_size);
extern void PICC_channel_incr_ref_count(PICC_Channel *ch);
extern void PICC_channel_dec_ref_count(PICC_Channel *ch);
extern PICC_KnownsSet *PICC_knowns_set_knows(PICC_KnownsSet *ks);
extern PICC_KnownsSet *PICC_knowns_set_forget(PICC_KnownsSet *ks);
extern void PICC_knowns_set_forget_to_unknown(PICC_KnownsSet *ks, PICC_Channel *ch);
extern void PICC_knowns_set_forget_all(PICC_KnownsSet *ks);
extern bool PICC_knowns_register(PICC_KnownsSet *ks, PICC_Channel *ch);
extern void PICC_release_all_channels(PICC_Channel **chans, int nb_chans);
extern void PICC_Channel_inv(PICC_Channel *channel);

#endif
