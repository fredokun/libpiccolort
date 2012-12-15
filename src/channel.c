/**
 * @file channel.c
 * Channels.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <channel.h>

/**
 * Creates a channel which contains 10 commitments.
 *
 * @return Created channel
 */
PICC_Channel *PICC_create_channel(PICC_Error *error)
{
    PICC_Channel *channel = (PICC_Channel *)malloc( sizeof(PICC_Channel));
    channel->global_rc = 1;
    channel->incommits = (PICC_Commit *) malloc( sizeof( PICC_Commit ) * 10 );
    channel->outcommits = (PICC_Commit *) malloc( sizeof( PICC_Commit ) * 10 );

    return channel;

}

/**
 * Create a channel which contains commit_size commitments
 *
 * @return created channel
 */
PICC_Channel *PICC_create_channel_cn( int commit_size )
{
    PICC_Channel * channel = (PICC_Channel *)malloc( sizeof(PICC_Channel));
    channel->global_rc = 1;
    channel->incommits = (PICC_Commit *) malloc( sizeof( PICC_Commit ) * commit_size );
    channel->outcommits = (PICC_Commit *) malloc( sizeof( PICC_Commit ) * commit_size );
    channel->lock = PICC_create_atomic_boolean();
    return channel;

}
