/**
 * @file value_test.c
 * Test of values
 *
 * This project is released under MIT License.
 *
 * @author Loïc Girault
 * @author Dany SIRIPHOL
 */

#include <stdlib.h>
#include <stdio.h>
#include <value_repr.h>


#define WITH_TRACE 1

void test_int(PICC_Error *error)
{
    PICC_Value v, v2, vresult;
    PICC_IntValue *i, *i2, *r;
    
    i=(PICC_IntValue *)&v;
    i2=(PICC_IntValue *)&v2;
    r=(PICC_IntValue *)&vresult;

    PICC_INIT_INT_VALUE(&v, 3);
    PICC_INIT_INT_VALUE(&v2, 4);
    PICC_INIT_INT_VALUE(&vresult, 0);


#ifdef WITH_TRACE
    printf("v = %d , v2 = %d, vresult = %d \n", i->data, i2->data, r->data);
#endif

    v=v2;

    ASSERT(i->data == i2->data)

#ifdef WITH_TRACE
    printf("v = %d , v2 = %d, vresult = %d \n", i->data, i2->data, r->data);
#endif
    
    
    PICC_Int_add(&vresult,&v,&v2);

    PICC_Int_multiply(&vresult,&v,&v2);

    PICC_Int_divide(&vresult,&v,&v2);

    PICC_Int_substract(&vresult,&v,&v2);

    PICC_equals(&vresult, &v, &v2);

    ASSERT(PICC_BOOL_OF_BOOL_VALUE(&vresult) == true);

    PICC_INIT_INT_VALUE(&v2, 12);
        
    PICC_equals(&vresult, &v, &v2);

    ASSERT(PICC_BOOL_OF_BOOL_VALUE(&vresult) == false);

    /* i = PICC_create_int_value(-3); */
    /* i2 = PICC_create_int_value(-12); */

    /* PICC_Int_add(result,i,i2); */
    /* PICC_Int_multiply(result,i,i2); */
    /* PICC_Int_divide(result,i,i2); */
    /* PICC_Int_substract(result,i,i2); */

    /* ASSERT( PICC_copy_value(&i2, i)); */
    
    /* PICC_Int_multiply(result,i,i2); */

    /* ASSERT( PICC_copy_value(&i, result)); */
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
