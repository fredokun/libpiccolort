/**
 * @file value.c
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <value.h>
#include <tools.h>

/**
 * Creates a new value of given type.
 *
 * @param type Type of the value
 * @param error Error stack
 * @return Created value
 */
PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error)
{
    PICC_ALLOC(value, PICC_Value, error) {
        value->kind = type;
        PICC_init_mutex(&(value->lock));
    }
    return value;
}

PICC_Value *PICC_create_value_int(int i, PICC_Error *error)
{
    PICC_Value* value = PICC_create_value(PICC_INT_VAL,error);
    value->content.as_int = i;
    return value;
}

/**
 * Creates a new clock.
 *
 * @param error Error stack
 * @return Created clock
 */
PICC_Clock *PICC_create_clock(PICC_Error *error)
{
    PICC_ALLOC(clock, PICC_Clock, error) {
        ALLOC_ERROR(sub_error);
        clock->val = PICC_create_atomic_int(&sub_error);
        if (HAS_ERROR(sub_error)) {
            ADD_ERROR(error, sub_error, ERR_CLOCK_CREATE);
            free(clock);
            clock = NULL;
        }
    }
    return clock;
}

/**
 * Frees the given clock.
 */
void PICC_reclaim_clock(PICC_Clock *clock)
{
    PICC_free_atomic_int(clock->val);
    free(clock);
}

