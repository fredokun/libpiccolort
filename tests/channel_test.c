/**
 * @file channel_test.c
 * Test of channels.
 *
 * This project is released under MIT License.
 *
 * @author Dany SIRIPHOL
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <pi_thread.h>
#include <runtime.h>

#define TESTS

/**
 * Test : PICC_create_pithread
 * PICC_PiThread *PICC_create_pithread();
 * @post channel != null && channel2 != null && channel3 == null
 * @return boolean true if it works else false
 */
bool test_create_channel(PICC_Error *error)
{ 
    PICC_Channel* channel = PICC_create_channel(error);
    PICC_Channel* channel2 = PICC_create_channel_cn(50,error);
    if (channel == NULL) {
        NEW_ERROR(error,ERR_NULLPOINTER_CHANNEL);
    }
    else if (channel2 == NULL) {
        NEW_ERROR(error,ERR_NULLPOINTER_CHANNEL);
    } 
    else if(channel->global_rc != 1 && channel2->global_rc != 1 )
    {
        NEW_ERROR(error,ERR_CHANNEL_GLOBAL_RC);
    }
    else {
        PICC_reclaim_channel(channel,error);
        PICC_reclaim_channel(channel2,error);
        return true;
    }
    
    return false;
}

bool test_global_reference(PICC_Error *error)
{
    PICC_Channel* channel = PICC_create_channel(error);

    PICC_channel_incr_ref_count(channel,error);
    PICC_channel_incr_ref_count(channel,error);
    PICC_channel_incr_ref_count(channel,error);

    if(channel->global_rc != 4)
    {
        NEW_ERROR(error,ERR_CHANNEL_GLOBAL_RC);
        return false;
    }

    PICC_channel_dec_ref_count(channel,error);
    PICC_channel_dec_ref_count(channel,error);

    if(channel->global_rc != 2)
    {
        NEW_ERROR(error,ERR_CHANNEL_GLOBAL_RC);
        return false;
    }

    PICC_channel_dec_ref_count(channel,error);

    if(channel->global_rc != 1)
    {
        NEW_ERROR(error,ERR_CHANNEL_GLOBAL_RC);
        return false;
    }

    PICC_channel_dec_ref_count(channel,error);

    if(channel->global_rc != 0)
    {
        NEW_ERROR(error,ERR_CHANNEL_GLOBAL_RC);
        return false;
    }

    return true;
}

bool test_channel_send(PICC_Error *error)
{
    PICC_Channel* channel = PICC_create_channel(error);
    return true;
}

void PICC_test_channels()
{
    ALLOC_ERROR(error);
    if(!test_create_channel(&error))
    {
        printf("test create_channel fail\n");
    }
    else
    {
        printf("test create_channel success \n");
    }
    if(!test_global_reference(&error))
    {
        printf("test channel global reference fail\n");
    }
    else
    {
        printf("test channel global reference success \n");
    }
    PRINT_ERROR(&error);
}

