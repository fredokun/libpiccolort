/**
 * @file value.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Dany SIRIPHOL
 * @author Loïc Girault
 */

#ifndef VALUE_H
#define VALUE_H

#include <channel.h>
#include <error.h>

/*******************************
 * Common value representation *
 *******************************/   

typedef struct _PICC_Value PICC_Value ;

PICC_Value* PICC_free_value(PICC_Value *v);
bool PICC_copy_value(PICC_Value **to, PICC_Value *from);
int PICC_compare_values(PICC_Value * value1, PICC_Value * value2);

void PICC_equals(PICC_Value *res, PICC_Value * value1, PICC_Value * value2);

/******************************
 * Immediate values : No value *
 ******************************/

typedef struct _no_value_t PICC_NoValue;

extern PICC_Value *PICC_create_no_value();

/******************************
 * Immediate values : boolean *
 ******************************/

typedef struct _bool_value_t PICC_BoolValue;

extern PICC_Value *PICC_create_bool_value(bool boolean);

// needed in some compilation schema
extern bool PICC_bool_of_bool_value(PICC_Value *val);

// boolean primitives

extern void PICC_Bool_and( PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Bool_or ( PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Bool_xor( PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Bool_not( PICC_Value *res, PICC_Value *v);

/******************************
 * Immediate values : integer *
 ******************************/

typedef struct _int_value_t PICC_IntValue;

extern PICC_Value *PICC_create_int_value(int data);

// integer primitives

extern void PICC_Int_add      (PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Int_multiply (PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Int_divide   (PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Int_substract(PICC_Value *res, PICC_Value *v1, PICC_Value *v2);

extern void PICC_Int_less_than(PICC_Value *res, PICC_Value *v1, PICC_Value *v2);
extern void PICC_Int_modulo(PICC_Value *res, PICC_Value *v1, PICC_Value *v2);

/******************************
 * Immediate values : float *
 ******************************/

 typedef struct _float_value_t PICC_FloatValue ;

/* extern PICC_FloatValue *PICC_create_float_value(double data); */
/* extern PICC_FloatValue *PICC_free_float(PICC_IntValue *val); */
/* extern void PICC_FloatValue_inv(PICC_FloatValue *val); */

/* // float primitives */

/* extern void PICC_Float_add(PICC_FloatValue *val, double value); */
/* extern void PICC_Float_multiply(PICC_FloatValue *val, double value); */
/* extern void PICC_Float_divide(PICC_FloatValue *val, double value); */
/* extern void PICC_Float_substract(PICC_FloatValue *val, double value); */

/******************
 * Tuples values  *
 ******************/

typedef struct _tuple_value_t PICC_TupleValue ;

/* extern PICC_TupleValue *PICC_create_tuple_value(int arity); */
/* extern PICC_TupleValue *PICC_free_tuple(PICC_TupleValue *tuple); */
/* extern void PICC_TupleValue_inv(PICC_TupleValue *tuple); */

/******************
 * String values  *
 ******************/

typedef struct _string_handle_t PICC_StringHandle;
typedef struct _string_value_t PICC_StringValue ;

extern PICC_Value *PICC_create_string_value( char *string );
extern PICC_StringValue *PICC_create_empty_string_value();

/******************
 * Channel values  *
 ******************/

typedef struct _channel_value_t PICC_ChannelValue ;

extern PICC_Value *PICC_create_channel_value(PICC_Channel* channel);
extern PICC_Channel *PICC_channel_of_channel_value(PICC_Value* channel); 

//extern void PICC_channel_value_release(PICC_VALUE* channel); // never explicitly released ?

extern void PICC_print_value_infos(PICC_Value * value);
extern void PICC_print_value(PICC_Value * value);
/**********************************
 * user defined immediate values  *
 **********************************/

/* typedef struct _user_immediate_value_t PICC_ImmediateValue ; */

/* extern PICC_ImmediateValue *PICC_create_immediate_value(int size); */
/* extern PICC_ImmediateValue *PICC_free_immediate_value( PICC_ImmediateValue *value); */
/* extern void PICC_ImmediateValue_inv(PICC_ImmediateValue *value); */

/**********************************
 * user defined managed values  *
 **********************************/

/* typedef struct _user_managed_channel_value_t PICC_ManagedValue ; */

/**********************************
 * user defined immediate values  *
 **********************************/


/* extern PICC_ManagedValue *PICC_create_managed_value(int size); */
/* extern PICC_ManagedValue *PICC_free_managed_value( PICC_ManagedValue *value); */
/* extern void PICC_ManagedValue_inv(PICC_ManagedValue *value); */

/*
extern PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error);
extern PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error);
extern PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error);
extern PICC_Clock *PICC_create_clock(PICC_Error *error);
*/

#endif
