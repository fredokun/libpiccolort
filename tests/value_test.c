/**
 * @file value_test.c
 * Test of values
 *
 * This project is released under MIT License.
 *
 * @author Dany SIRIPHOL
 */

#include <stdlib.h>
#include <stdio.h>
#include <value_repr.h>


void test_int(PICC_Error *error)
{
    PICC_Value *i = PICC_create_int_value(3);
    PICC_Value *i2 = PICC_create_int_value(4);
    PICC_Value *result = PICC_create_int_value(0);
    PICC_Int_add(result,i,i2);
    PICC_Int_multiply(result,i,i2);
    PICC_Int_divide(result,i,i2);
    PICC_Int_substract(result,i,i2);

    PICC_free_int((PICC_IntValue *)i);
    PICC_free_value(i2);
    
    i = PICC_create_int_value(-3);
    i2 = PICC_create_int_value(-12);

    PICC_Int_add(result,i,i2);
    PICC_Int_multiply(result,i,i2);
    PICC_Int_divide(result,i,i2);
    PICC_Int_substract(result,i,i2);

    ASSERT( PICC_copy_value(&i2, i));
    
    PICC_Int_multiply(result,i,i2);

    ASSERT( PICC_copy_value(&i, result));
}

void test_bool(PICC_Error *error)
{
    PICC_Value *b = PICC_create_bool_value(true);
    PICC_Value *b2 = PICC_create_bool_value(false);
    PICC_Value *result = PICC_create_bool_value(false);

    PICC_Bool_and(result,b,b2);
    PICC_Bool_or(result,b,b2);
    PICC_Bool_xor(result,b,b2);
    PICC_Bool_not(result,b);

    b = PICC_create_bool_value(false);
    b2 = PICC_create_bool_value(false);

    PICC_Bool_and(result,b,b2);
    PICC_Bool_or(result,b,b2);
    PICC_Bool_xor(result,b,b2);

    b = PICC_create_bool_value(false);
    b2 = PICC_create_bool_value(true);

    PICC_Bool_and(result,b,b2);
    PICC_Bool_or(result,b,b2);
    PICC_Bool_xor(result,b,b2);
    PICC_Bool_not(result,b);

    ASSERT( PICC_copy_value(&b, b2));
    
    PICC_Bool_not(result,b);

    ASSERT( PICC_copy_value(&b, result));
}

void test_string(PICC_Error *error)
{
    PICC_Value *s = PICC_create_string_value("test");
    PICC_Value *s2 = PICC_create_string_value("test2");
    PICC_Value *result = (PICC_Value *) PICC_create_empty_string_value();

    ASSERT( PICC_copy_value(&result, s2));
    ASSERT( PICC_copy_value(&result, s));
    result = (PICC_Value *) PICC_create_empty_string_value();
    PICC_free_value(result);
}

/*void test_tuples(PICC_Error *error)
{
    PICC_Value *s = PICC_create_string_value("test");
    PICC_Value *s2 = PICC_create_string_value("test2");
    PICC_Value *tuple = PICC_TupleValue *PICC_create_tuple_value(2);
}*/

/**
 * Runs all value tests.
 */
void PICC_test_value()
{
    ALLOC_ERROR(error);
    test_int(&error);
    test_bool(&error);
    test_string(&error);
    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}
