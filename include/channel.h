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

extern void PICC_release_all_channels(PICC_KnownSet *chans);
extern void PICC_Channel_inv(PICC_Channel *channel);

#endif
