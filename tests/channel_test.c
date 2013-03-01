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
 * Test : channel creation
 *
 * Check if created channel has correct commitment size and correct global reference count
 * @return boolean true if it works else false
 */
bool test_create_channel(PICC_Error *error)
{
    PICC_Channel *channel = PICC_create_channel(error);
    PICC_Channel *channel2 = PICC_create_channel_cn(50,20);

    PICC_reclaim_channel(channel,error);
    PICC_reclaim_channel(channel2,error);
    return true;
    

}

/**
 * Test : channel global reference
 *
 * Check if global reference field is correctly incremented and decremented \n
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
    PICC_Channel* channel = PICC_create_channel();

    int i = 0;
    PICC_Knowns *knowns;
    
    for (i=0;i<10;i++)
    {
        knowns = PICC_create_knowns(channel,error);
        set->knowns[i] = knowns;
    }

    return true;
}

PICC_Value *eval_int(PICC_Error *error)
{
    /*PICC_Value *value = PICC_create_value_int(2,error);
    return value;*/
    return NULL;
}

bool test_channel_send(PICC_Error *error)
{
  //  PICC_Channel* channel = PICC_create_channel(error);
    PICC_Value * v = eval_int(error);
   // ASSERT(v->content.as_int == 2);
    return true;
}


/**
 * Launch all channel tests
 */
void PICC_all_channel_test()
{
    ALLOC_ERROR(error);
    test_create_channel(&error);
    printf("test create_channel success \n");
    test_global_reference(&error);
    printf("test global_reference success \n");
    test_channel_send(&error);
    printf("test channel send success \n");
    test_knowsSet(&error);
    if(HAS_ERROR(error))
    {
         CRASH(&error);
    }
   
}

