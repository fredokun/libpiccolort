/**
 * @file value.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Dany SIRIPHOL
 */

#ifndef VALUE_H
#define VALUE_H

#include <channel.h>
#include <error.h>

/*******************************
 * Common value representation *
 *******************************/


typedef struct _PICC_Value PICC_Value ;

/******************************
 * Immediate values : No value *
 ******************************/

typedef struct _no_value_t PICC_NoValue;

extern PICC_NoValue *PICC_create_no_value();
extern PICC_NoValue *PICC_free_no_value(PICC_NoValue * val);
extern void PICC_no_value_inv(PICC_NoValue * val);

/******************************
 * Immediate values : boolean *
 ******************************/

typedef struct _bool_value_t PICC_BoolValue;

extern PICC_BoolValue *PICC_create_bool_value(int boolean);
extern PICC_BoolValue *PICC_free_bool(PICC_BoolValue * val);
extern void PICC_bool_inv(PICC_BoolValue * val);

/******************************
 * Immediate values : integer *
 ******************************/

typedef struct _int_value_t PICC_IntValue;

extern PICC_IntValue *PICC_create_int_value(int data);
extern PICC_IntValue *PICC_free_int(PICC_IntValue * val);
extern void PICC_int_inv(PICC_IntValue * val);

/******************************
 * Immediate values : float *
 ******************************/

typedef struct _float_value_t PICC_FloatValue ;

extern PICC_FloatValue *PICC_create_float_value(double data);
extern PICC_FloatValue *PICC_free_float(PICC_IntValue * val);
extern void PICC_float_inv(PICC_FloatValue * val);

/******************
 * Tuples values  *
 ******************/

typedef struct _tuple_value_t PICC_TupleValue ;

extern PICC_TupleValue *PICC_create_tuple_value(int arity);
extern PICC_TupleValue *PICC_free_tuple(PICC_TupleValue *tuple);
extern void PICC_TupleValue_inv(PICC_TupleValue *tuple);

/******************
 * String values  *
 ******************/

typedef struct _string_handle_t PICC_StringHandle;
typedef struct _string_value_t PICC_StringValue ;

extern PICC_StringValue *PICC_create_string_value( char *string );
extern PICC_StringValue *PICC_free_string( PICC_StringValue *string);
extern void PICC_StringValue_inv(PICC_StringValue *string);


/******************
 * Channel values  *
 ******************/

typedef struct _channel_value_t PICC_ChannelValue ;

extern PICC_ChannelValue *PICC_create_channel_value();
extern PICC_ChannelValue *PICC_free_channel_value( PICC_ChannelValue *channel);
extern void PICC_ChannelValue_inv(PICC_ChannelValue *channel);

/**********************************
 * user defined immediate values  *
 **********************************/

typedef struct _user_immediate_value_t PICC_ImmediateValue ;

extern PICC_ImmediateValue *PICC_create_immediate_value(int size);
extern PICC_ImmediateValue *PICC_free_immediate_value( PICC_ImmediateValue *value);
extern void PICC_ImmediateValue_inv(PICC_ImmediateValue *value);

/**********************************
 * user defined managed values  *
 **********************************/

typedef struct _user_managed_channel_value_t PICC_ManagedValue ;

/**********************************
 * user defined immediate values  *
 **********************************/

typedef struct _PICC_Clock PICC_Clock;

extern PICC_ManagedValue *PICC_create_managed_value(int size);
extern PICC_ManagedValue *PICC_free_managed_value( PICC_ManagedValue *value);
extern void PICC_ManagedValue_inv(PICC_ManagedValue *value);

extern PICC_Clock *PICC_create_clock(PICC_Error *error);
/*
extern PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error);
extern PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error);
extern PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error);
extern PICC_Clock *PICC_create_clock(PICC_Error *error);
*/

#endif
