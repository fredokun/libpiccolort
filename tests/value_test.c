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

    PICC_free_value(i);
    PICC_free_value(result);
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
    PICC_Value *result2;

    ASSERT( PICC_copy_value(&result, s2));
    ASSERT( PICC_copy_value(&result, s));
    ASSERT( PICC_copy_value(&result2, s));

    result = (PICC_Value *) PICC_create_empty_string_value();
    PICC_free_value(result);
    PICC_free_value(result2);
}

void test_tuples(PICC_Error *error)
{
    int arity = 2;

    PICC_Value *s = PICC_create_string_value("test");
    PICC_Value *s2 = PICC_create_string_value("test2");
    PICC_Value **elements = malloc(sizeof(PICC_Value)*arity);

    elements[0] = s;
    elements[1] = s2;

    PICC_Value *tuple = (PICC_Value *)PICC_create_tuple_value(arity);
    PICC_set_tuple_elements(tuple,elements);

    ASSERT(PICC_get_tuple_element(tuple,0) == s);
    ASSERT(PICC_get_tuple_element(tuple,1) == s2);

    PICC_StringValue_inv((PICC_StringValue *)PICC_get_tuple_element(tuple,0));
    PICC_StringValue_inv((PICC_StringValue *)PICC_get_tuple_element(tuple,1));

    PICC_Value *tuple2 = (PICC_Value *)PICC_create_tuple_value(arity);
    PICC_Value *tuple3 ;
    ASSERT(PICC_copy_value(&tuple2,tuple));
    ASSERT(PICC_copy_value(&tuple3,tuple));

    PICC_StringValue_inv((PICC_StringValue *)PICC_get_tuple_element(tuple2,0));
    PICC_StringValue_inv((PICC_StringValue *)PICC_get_tuple_element(tuple2,1));

    PICC_StringValue_inv((PICC_StringValue *)PICC_get_tuple_element(tuple3,0));
    PICC_StringValue_inv((PICC_StringValue *)PICC_get_tuple_element(tuple3,1));

    PICC_Value *tuple4 = (PICC_Value *)PICC_create_tuple_value(0);

    PICC_free_value(tuple);
    PICC_free_value(tuple2);
    PICC_free_value(tuple3);
    PICC_free_value(tuple4);

    PICC_free_value(s);
    PICC_free_value(s2);
}

void test_channels(PICC_Error *error)
{
    PICC_Channel *channel= PICC_create_channel_cn(50,20);
    PICC_Value *cv = PICC_create_channel_value(channel);
    PICC_Value *cv2= PICC_create_empty_channel_value(PI_CHANNEL);

    PICC_Value *cv3;

    ASSERT( PICC_copy_value(&cv3, cv));
    ASSERT( PICC_copy_value(&cv3, cv2));
  
    PICC_free_value(cv);
    PICC_free_value(cv2);
    PICC_free_value(cv3);
}

/**
 * Runs all value tests.
 */
void PICC_test_value()
{
    ALLOC_ERROR(error);
    test_int(&error);
    test_bool(&error);
    test_string(&error);
    test_tuples(&error);
    test_channels(&error);
    if (HAS_ERROR(error))
        PRINT_ERROR(&error);
}
