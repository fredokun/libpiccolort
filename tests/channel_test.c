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
#include <gc.h>
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
    PICC_Handle* handle = (PICC_Handle*) channel;

    PICC_handle_incr_ref_count(handle);
    PICC_handle_incr_ref_count(handle);
    PICC_handle_incr_ref_count(handle);

    ASSERT(handle->global_rc == 4);

    PICC_handle_dec_ref_count(&handle);
    PICC_handle_dec_ref_count(&handle);

    ASSERT(handle->global_rc == 2);

    PICC_handle_dec_ref_count(&handle);

    ASSERT(handle->global_rc == 1);

    PICC_handle_dec_ref_count(&handle);
}



/**
 * Runs all channel tests.
 */
void PICC_test_channel()
{
    ALLOC_ERROR(error);
    test_create_channel(&error);
    test_global_reference(&error);

    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}
