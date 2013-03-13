/**
 * @file channel_test.c
 * Test of channels.
 *
 * This project is released under MIT License.
 *
 * @author Dany SIRIPHOL
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <pi_thread_repr.h>
#include <channel_repr.h>
#include <value_repr.h>
#include <knownset_repr.h>

#define ASSERT_NO_ERROR() \
 ASSERT(!HAS_ERROR((*error)))


/**
 * Test : channel creation
 *
 * Check if created channel has correct commitment size and correct global reference count
 * @return boolean true if it works else false
 */
void test_create_channel(PICC_Error *error)
{
    PICC_Channel *channel = PICC_create_channel(error);
    ASSERT_NO_ERROR();
    PICC_Channel *channel2 = PICC_create_channel_cn(50,20);

    PICC_reclaim_channel(channel, error);
    ASSERT_NO_ERROR();
    PICC_reclaim_channel(channel2, error);
    ASSERT_NO_ERROR();
}

/**
 * Test : channel global reference
 *
 * Check if global reference field is correctly incremented and decremented \n
 */
void test_global_reference(PICC_Error *error)
{
    PICC_Channel* channel = PICC_create_channel();

    PICC_channel_incr_ref_count(channel);
    PICC_channel_incr_ref_count(channel);
    PICC_channel_incr_ref_count(channel);

    ASSERT(channel->global_rc == 4);

    PICC_channel_dec_ref_count(&channel);
    PICC_channel_dec_ref_count(&channel);

    ASSERT(channel->global_rc == 2);

    PICC_channel_dec_ref_count(&channel);

    ASSERT(channel->global_rc == 1);

    PICC_channel_dec_ref_count(&channel);
}

/**
 * Test : PICC_KnownSet and PICC_Knowns creation \n
   check if knownsSet is created with the right size and check if \n
    knowns type is created with PICC_UNKNOWN state
 */
void test_knowsSet(PICC_Error *error)
{
    PICC_KnownSet *set = PICC_create_known_set(10, error);
    ASSERT_NO_ERROR();
    PICC_Channel* channel = PICC_create_channel();

    int i = 0;
    PICC_Knowns *knowns;

    for (i=0;i<10;i++)
    {
        knowns = PICC_create_knowns(PICC_create_channel_value(channel), error);
        ASSERT_NO_ERROR();
        PICC_known_set_add(set, knowns->val);
    }
}

PICC_Value *eval_int(PICC_Error *error)
{
    PICC_Value *value = (PICC_Value *)PICC_create_int_value(2);
    return value;

}

void test_channel_send(PICC_Error *error)
{
  //  PICC_Channel* channel = PICC_create_channel(error);
    PICC_Value *v = eval_int(error);
    PICC_IntValue_inv((PICC_IntValue *)v);
}

/**
 * Runs all channel tests.
 */
void PICC_test_channel()
{
    ALLOC_ERROR(error);
    test_create_channel(&error);
    test_global_reference(&error);
    test_channel_send(&error);
    //test_knowsSet(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}
