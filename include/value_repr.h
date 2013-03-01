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
#include <channel.h>
#include <atomic.h>
#include <concurrent.h>
#include <error.h>

/****************************
 * Tag & control management *
 ***************************/

typedef enum { TAG_RESERVED=0, 
               TAG_NOVALUE=1, 
               TAG_BOOLEAN=2,
               TAG_INTEGER=3,
               TAG_FLOAT=4,
               TAG_TUPLE=64,
               TAG_STRING=128,
               TAG_CHANNEL=253,
               TAG_USER_DEFINED_IMMEDIATE=254,
               TAG_USER_DEFINED_MANAGED=255 } PICC_TagValue;

#define VALUE_HEADER unsigned int header

#define WORD_SIZE 32
#define GET_VALUE_TAG(header) ((unsigned int) ((header) >> (WORD_SIZE - 8)))
#define VALUE_CTRL_MASK (~(0xFF << (WORD_SIZE - 8) ))
#define GET_VALUE_CTRL(header) ((header) & VALUE_CTRL_MASK)

#define MAKE_HEADER(tag,ctrl) ((unsigned int) (((tag) << (WORD_SIZE - 8)) | ((ctrl) & VALUE_CTRL_MASK)))

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

#define MAKE_NO_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_NOVALUE,0) })

/******************************
 * Immediate values : integer *
 ******************************/

#define MAKE_INT_VALUE(data) ((PICC_IntValue)  { MAKE_HEADER(TAG_INTEGER,0), ((int) (data)) })

struct _int_value_t {
    VALUE_HEADER ;
    int data;
};

/******************************
 * Immediate values : boolean *
 ******************************/

struct _bool_value_t {
  VALUE_HEADER;
};

#define MAKE_TRUE_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_BOOLEAN,1) })
#define MAKE_FALSE_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_BOOLEAN,0) })

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

PICC_StringHandle *string_handles;

struct _string_value_t {
    VALUE_HEADER;
    PICC_StringHandle *handle;
};

struct _string_handle_t 
{
    int refcount;
    char *data;
};


extern void PICC_init_string_handles(int arity);
extern PICC_StringHandle *PICC_create_string_handle(char *string);
extern void PICC_StringHandle_inv(PICC_StringHandle *handle);


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

struct _channel_value_t {
    VALUE_HEADER ;
    void *channel;
};

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


#endif
