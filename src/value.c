/**
 * @file value.c
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <value_repr.h>
#include <atomic_repr.h>
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
        PICC_init_lock(&(value->lock));
    }
    return value;
}

PICC_Value *PICC_create_value_int(int i, PICC_Error *error)
{
    PICC_Value* value = PICC_create_value(PICC_INT_VAL,error);
    value->content.as_int = i;
    return value;
}
