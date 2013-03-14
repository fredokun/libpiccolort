/**
 * @file value_repr.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Dany SIRIPHOL
 */

#ifndef VALUE_REPR_H
#define VALUE_REPR_H

#include <value.h>
#include <gc.h>
#include <channel.h>
#include <atomic.h>
#include <atomic_repr.h>
#include <concurrent.h>
#include <error.h>

/****************************
 * Tag & control management *
 ***************************/

typedef enum { TAG_RESERVED               =0x00,
               TAG_NOVALUE                =0x01,
               TAG_BOOLEAN                =0x02,
               TAG_INTEGER                =0x03,
               TAG_FLOAT                  =0x04,
               TAG_TUPLE                  =0x40,
               TAG_STRING                 =0x80,
               TAG_CHANNEL                =0xFD,
               TAG_USER_DEFINED_IMMEDIATE =0xFE,
               TAG_USER_DEFINED_MANAGED   =0xFF } PICC_TagValue;

#define VALUE_HEADER unsigned int header

#define WORD_SIZE 32
#define GET_VALUE_TAG(header) ( ((unsigned int) (header)) >> (WORD_SIZE - 8) )
#define VALUE_CTRL_MASK (~(0xFF << (WORD_SIZE - 8) ))

#define GET_VALUE_CTRL(header) ((header) & VALUE_CTRL_MASK)

#define MAKE_HEADER(tag,ctrl) ((unsigned int) (((tag) << (WORD_SIZE - 8)) | ((ctrl) & VALUE_CTRL_MASK)))


#define IS_NOVALUE(value) (GET_VALUE_TAG((value->header)) == TAG_NOVALUE)
#define IS_BOOLEAN(value) (GET_VALUE_TAG((value->header)) == TAG_BOOLEAN)
#define IS_INT(value)     (GET_VALUE_TAG((value->header)) == TAG_INTEGER)
#define IS_FLOAT(value)   (GET_VALUE_TAG((value->header)) == TAG_FLOAT)
//---
#define IS_STRING(value)  (GET_VALUE_TAG((value->header)) == TAG_STRING)
#define IS_CHANNEL(value) (GET_VALUE_TAG((value->header)) == TAG_CHANNEL)



/*******************************
 * Common value representation *
 *******************************/

struct _PICC_Value {
  VALUE_HEADER ;
};

/******************************
 * Immediate values :no value *
 ******************************/

struct _no_value_t {
    VALUE_HEADER ;
};

#define MAKE_NO_VALUE ((PICC_NoValue)  { MAKE_HEADER(TAG_NOVALUE,0) })
extern void PICC_NoValue_inv(PICC_NoValue *val);

/******************************
 * Immediate values : integer *
 ******************************/


struct _int_value_t {
    VALUE_HEADER ;
    int data;
};

#define MAKE_INT_VALUE(data) ((PICC_IntValue)  { MAKE_HEADER(TAG_INTEGER,0), ((int) (data)) })
extern void PICC_IntValue_inv(PICC_IntValue *val);
extern PICC_IntValue *PICC_free_int(PICC_IntValue *val);

/******************************
 * Immediate values : boolean *
 ******************************/

struct _bool_value_t {
  VALUE_HEADER;
};

#define MAKE_TRUE_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_BOOLEAN,1) })
#define MAKE_FALSE_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_BOOLEAN,0) })
extern void PICC_BoolValue_inv(PICC_BoolValue *val);

/******************************
 * Immediate values : float *
 ******************************/

struct _float_value_t {
  VALUE_HEADER;
  double data;
} ;

#define MAKE_FLOAT_VALUE(data) ((PICC_FloatValue)  { MAKE_HEADER(TAG_FLOAT,0), ((double) (data)) })

/******************
 * String values  *
 ******************/

struct _string_value_t {
    VALUE_HEADER;
    int index_in_knownset;
    PICC_StringHandle *handle;
};

struct _string_handle_t  //"implements PICC_KnownHandle"
{
    int global_rc; 
    PICC_Lock *lock;
    PICC_Reclaimer reclaim;
    char *data;
};

extern void PICC_StringValue_inv(PICC_StringValue *string);

extern PICC_StringHandle *PICC_create_string_handle(char *string);
extern void PICC_StringHandle_inv(PICC_StringHandle *handle);
extern PICC_StringValue *PICC_free_string( PICC_StringValue *string);


/******************
 * Tuples values  *
 ******************/


struct _tuple_value_t {
    VALUE_HEADER ;
    PICC_Value ** elements ;
};

/******************
 * Channel values  *
 ******************/

typedef enum {
    PI_CHANNEL =0
}PICC_ChannelKind;

typedef void PICC_ChannelHandle;

struct _channel_value_t {
    VALUE_HEADER ;
    int index_in_knownset;
    PICC_ChannelHandle *channel;
};

extern void PICC_ChannelValue_inv(PICC_ChannelValue *channel);
extern PICC_ChannelValue *PICC_free_channel_value( PICC_ChannelValue *channel);

/**********************************
 * user defined immediate values  *
 **********************************/

struct _user_immediate_value_t {
    VALUE_HEADER ;
    int *data;
};

/**********************************
 * user defined managed values  *
 **********************************/

struct _user_managed_channel_value_t {
    VALUE_HEADER ;
    int* data;
};

extern PICC_ChannelValue *PICC_create_pi_channel_value();
extern PICC_ChannelValue *PICC_create_typed_channel_value( PICC_ChannelKind kind );
extern void PICC_ChannelValue_inv(PICC_ChannelValue *channel);

extern void PICC_print_value_infos(PICC_Value * value);


#endif
