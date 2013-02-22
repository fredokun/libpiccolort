/**
 * @file atomic.c
 * Atomic booleans and integers.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <atomic_repr.h>
#include <tools.h>

#define LOCK_ATOMIC_VALUE(val) \
    pthread_mutex_lock(&(val->lock));

#define RELEASE_ATOMIC_VALUE(val) \
    pthread_mutex_unlock(&(val->lock));


// Atomic booleans /////////////////////////////////////////////////////////////

/**
 * Creates a new atomic boolean.
 *
 * @param error Error stack
 * @return Created atomic boolean
 */
PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error)
{
    PICC_ALLOC(atomic_bool, PICC_AtomicBoolean, error) {
        atomic_bool->val = false;
        pthread_mutex_init(&(atomic_bool->lock), NULL);
    }
    return atomic_bool;
}

/**
 * Frees an atomic boolean.
 *
 * @pre atomic_bool != null
 * @param atomic_bool Atomic boolean to be freed
 */
void PICC_free_atomic_bool(PICC_AtomicBoolean *atomic_bool)
{
    #ifdef CONTRACT
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    free(atomic_bool);
}

/**
 * Atomically sets the value to the given updated value if the current value
 * is equal to the expected value.
 *
 * @pre atomic_bool != null
 * @post if (atomic_bool.val@pre == expected_val) then atomic_bool.val = new_val
 * @param atomic_bool Atomic boolean to be set
 * @param expected_val Expected current value
 * @param new_val New value of the boolean
 * @return Value of the boolean before the operation
 */
bool PICC_atomic_bool_compare_and_swap(PICC_AtomicBoolean *atomic_bool, bool expected_val, bool new_val)
{
    LOCK_ATOMIC_VALUE(atomic_bool);

    #ifdef CONTRACT
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);

        // captures
        bool val_at_pre = atomic_bool->val;
    #endif CONTRACT

    bool old_val = atomic_bool->val;
    if (old_val == expected_val)
        atomic_bool->val = new_val;

    #ifdef CONTRACT
        // post: if (atomic_bool.val@pre == expected_val) then atomic_bool.val = new_val
        if (val_at_pre == expected_val)
            ASSERT(atomic_bool->val == new_val);
    #endif CONTRACT

    RELEASE_ATOMIC_VALUE(atomic_bool);
    return old_val;
}

/**
 * Unconditionally gets the current value of the atomic boolean value
 *
 * @pre atomic_bool != null
 * @param atomic_bool Atomic boolean to get
 * @return Current value of the atomic boolean
 */
bool PICC_atomic_bool_get(PICC_AtomicBoolean *atomic_bool)
{
    #ifdef CONTRACT
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif

    return atomic_bool->val;
}

/**
 * Unconditionally sets the atomic boolean value.
 *
 * @pre atomic_bool != null
 * @post atomic_bool.val == new_val
 * @param atomic_bool Atomic boolean to be set
 */
void PICC_atomic_bool_set(PICC_AtomicBoolean *atomic_bool, bool new_val)
{
    #ifdef CONTRACT
        // pre: atomic_bool != null
        ASSERT(atomic_bool != NULL);
    #endif CONTRACT

    atomic_bool->val = new_val;

    #ifdef CONTRACT
        // post: atomic_bool.val == new_val
        ASSERT(atomic_bool->val == new_val);
    #endif
}


// Atomic integers /////////////////////////////////////////////////////////////

/**
 * Creates a new atomic integer.
 *
 * @param error Error stack
 * @return Created atomic integer
 */
PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error)
{
    PICC_ALLOC(atomic_int, PICC_AtomicInt, error) {
        atomic_int->val = 0;
        pthread_mutex_init(&(atomic_int->lock), NULL);
    }
    return atomic_int;
}

/**
 * Frees an atomic integer.
 *
 * @pre atomic_int != null
 * @param atomic_int Atomic integer to be freed
 */
void PICC_free_atomic_int(PICC_AtomicInt *atomic_int)
{
    #ifdef CONTRACT
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    free(atomic_int);
}

/**
 * Atomically sets the value to the given updated value if the current value
 * is equal to the expected value.
 *
 * @pre atomic_int != null
 * @post if (atomic_int.val@pre == expected_val) then atomic_int.val = new_val
 * @param atomic_int Atomic integer to be set
 * @param expected_val Expected current value
 * @param new_val New value of the integer
 * @return Value of the integer before the operation
 */
int PICC_atomic_int_compare_and_swap(PICC_AtomicInt *atomic_int, int expected_val, int new_val)
{
    LOCK_ATOMIC_VALUE(atomic_int);

    #ifdef CONTRACT
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);

        // captures
        int val_at_pre = atomic_int->val;
    #endif CONTRACT

    int old_val = atomic_int->val;
    if (old_val == expected_val)
        atomic_int->val = new_val;

    #ifdef CONTRACT
        // post: if (atomic_int.val@pre == expected_val) then atomic_int.val = new_val
        if (val_at_pre == expected_val)
            ASSERT(atomic_int->val == new_val);
    #endif CONTRACT

    RELEASE_ATOMIC_VALUE(atomic_int);
    return old_val;
}

/**
 * Unconditionally gets the current value of the atomic integer value
 *
 * @pre atomic_int != null
 * @param atomic_int Atomic integer to get
 * @return Current value of the atomic integer
 */
int PICC_atomic_int_get(PICC_AtomicInt *atomic_int)
{
    #ifdef CONTRACT
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif

    return atomic_int->val;
}

/**
 * Unconditionally sets the atomic integer value.
 *
 * @pre atomic_int != null
 * @post atomic_int.val == new_val
 * @param atomic_int Atomic integer to be set
 */
void PICC_atomic_int_set(PICC_AtomicInt *atomic_int, int new_val)
{
    #ifdef CONTRACT
        // pre: atomic_int != null
        ASSERT(atomic_int != NULL);
    #endif CONTRACT

    atomic_int->val = new_val;

    #ifdef CONTRACT
        // post: atomic_int.val == new_val
        ASSERT(atomic_int->val == new_val);
    #endif
}
