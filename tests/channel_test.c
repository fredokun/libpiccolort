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
#include <value.h>

#define TESTS


/**
 * Test : channel creation\n
 * check if created channel has correct commitment size and correct global reference count
 * @return boolean true if it works else false
 */
bool test_create_channel(PICC_Error *error)
{
    PICC_Channel *channel = PICC_create_channel(error);
    PICC_Channel *channel2 = PICC_create_channel_cn(50,20);

    ASSERT(channel != NULL );
    ASSERT(channel->global_rc == 1);
    ASSERT(channel->incommits->size == DEFAULT_CHANNEL_COMMIT_SIZE );
    ASSERT(channel->outcommits->size == DEFAULT_CHANNEL_COMMIT_SIZE  );

    ASSERT(channel2 != NULL );
    ASSERT(channel2->global_rc == 1);
    ASSERT(channel2->incommits->size == 50 );
    ASSERT(channel2->outcommits->size == 20 );

    PICC_reclaim_channel(channel,error);
    PICC_reclaim_channel(channel2,error);
    return true;
    

}

/**
 * Test : channel global reference \n
 *   check if global reference field is correctly incremented and decremented \n
 * @return boolean true if it works else false
 */
bool test_global_reference(PICC_Error *error)
{
    PICC_Channel* channel = PICC_create_channel();

    PICC_channel_incr_ref_count(channel);
    PICC_channel_incr_ref_count(channel);
    PICC_channel_incr_ref_count(channel);

    ASSERT(channel->global_rc == 4);

    PICC_channel_dec_ref_count(channel);
    PICC_channel_dec_ref_count(channel);

    ASSERT(channel->global_rc == 2);

    PICC_channel_dec_ref_count(channel);

    ASSERT(channel->global_rc == 1);

    PICC_channel_dec_ref_count(channel);

    if(channel->global_rc != 0)
    {
        NEW_ERROR(error,ERR_CHANNEL_GLOBAL_RC);
        return false;
    }

    return true;
}

/**
 * Test : PICC_KnownsSet and PICC_Knowns creation \n
   check if knownsSet is created with the right size and check if \n
    knowns type is created with PICC_UNKNOWN state
 * @return boolean true if it works else false
 */
bool test_knowsSet(PICC_Error *error)
{
    PICC_KnownsSet *set = PICC_create_knowns_set(10, error);

    ASSERT(set != NULL );
    ASSERT(set->length == 10);

    int i = 0;
    PICC_Knowns *knowns;
    
    for (i=0;i<10;i++)
    {
        knowns = PICC_create_knowns(error);
        set->knowns[i] = knowns;
        ASSERT(set->knowns[i]->state == PICC_UNKNOWN);
    }

    return true;
}

bool test_knowns

PICC_Value *eval_int(PICC_Error *error)
{
    PICC_Value *value = PICC_create_value_int(2,error);
    return value;
}

bool test_channel_send(PICC_Error *error)
{
  //  PICC_Channel* channel = PICC_create_channel(error);
    PICC_Value * v = eval_int(error);
    ASSERT(v->content.as_int == 2);
    return true;
}


/**
 * Launch all channel tests
 */
void PICC_all_channel_test()
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
    if(!test_channel_send(&error))
    {
        printf("test channel send fail\n");
    }
    else
    {
        printf("test channel send success \n");
    }
    test_knowsSet(&error);
    if(HAS_ERROR(error))
    {
         CRASH(&error);
    }
   
}

