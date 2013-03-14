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
#include <knownset.h>

/**
 * The type of the pi-thread channels
 */
typedef struct _PICC_Channel PICC_Channel;

extern PICC_Channel *PICC_create_channel();
extern PICC_Channel *PICC_create_channel_cn(int incommit_size,int outcommit_size);
extern void PICC_channel_incr_ref_count(PICC_Channel *ch);
extern void PICC_channel_dec_ref_count(PICC_Channel **ch);

/* extern void PICC_release_all_channels(PICC_KnownSet *chans); */
/* extern void PICC_knowns_set_forget_to_unknown(PICC_KnownSet *ks, PICC_Channel *c); */
/* extern bool PICC_knowns_register(PICC_KnownSet *ks, PICC_Channel *c); */

/* #define PICC_CHANNEL_KNOWNSET_FOREACH(channel, set, it)		        \ */
/*         do{								\ */
/*         PICC_Channel* channel;						        \ */
/*         PICC_KnownSetIterator *it = PICC_create_known_set_iterator(set);	\ */
/*         while(PICC_known_set_has_next(it)){				\ */
/* 	    channel = PICC_channel_of_channel_value((PICC_Value*) PICC_known_set_next(it)) */

extern void PICC_Channel_inv(PICC_Channel *channel);

#endif
