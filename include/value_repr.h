/**
 * @file value_repr.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 * @author Dany SIRIPHOL
 * @author Loïc Girault
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
    void* data; //dummy value so all the value structures have the same size
};

/******************************
 * Immediate values :no value *
 ******************************/

struct _no_value_t {
    VALUE_HEADER ;
    void* data;
};

#define PICC_INIT_NO_VALUE(val)						\
    do{									\
	(val)->header = MAKE_HEADER(TAG_NOVALUE,0);			\
	(val)->data = 0;						\
    }while(0)


/* #define MAKE_NO_VALUE ((PICC_NoValue)  { MAKE_HEADER(TAG_NOVALUE,0) }) */
extern void PICC_NoValue_inv(PICC_NoValue *val);

/******************************
 * Immediate values : integer *
 ******************************/


struct _int_value_t {
    VALUE_HEADER ;
    int data;
};

#define PICC_INIT_INT_VALUE(val, i)					\
    do{									\
	(val)->header = MAKE_HEADER(TAG_INTEGER,0);			\
	((PICC_IntValue*) (val))->data = (i);				\
    }while(0)


/* #define MAKE_INT_VALUE(data) ((PICC_IntValue)  { MAKE_HEADER(TAG_INTEGER,0), ((int) (data)) }) */
extern void PICC_IntValue_inv(PICC_IntValue *val);
extern PICC_IntValue *PICC_free_int(PICC_IntValue *val);

/******************************
 * Immediate values : boolean *
 ******************************/

struct _bool_value_t {
    VALUE_HEADER;
    void* data;
};

#define PICC_INIT_BOOL_VALUE(val, i)					\
    do{									\
	(val)->header = MAKE_HEADER(TAG_BOOLEAN,(i));			\
	((PICC_BoolValue*) (val))->data = 0;						\
    }while(0)

#define PICC_INIT_BOOL_TRUE(val) PICC_INIT_BOOL_VALUE(val, 1)
#define PICC_INIT_BOOL_FALSE(val) PICC_INIT_BOOL_VALUE(val, 0)

#define PICC_BOOL_OF_BOOL_VALUE(val) GET_VALUE_CTRL((val)->header)

/* #define MAKE_TRUE_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_BOOLEAN,1) }) */
/* #define MAKE_FALSE_VALUE ((PICC_BoolValue)  { MAKE_HEADER(TAG_BOOLEAN,0) }) */
extern void PICC_BoolValue_inv(PICC_BoolValue *val);

/******************************
 * Immediate values : float *
 ******************************/

struct _float_value_t {
  VALUE_HEADER;
  double* data;
};

//#define MAKE_FLOAT_VALUE(data) ((PICC_FloatValue)  { MAKE_HEADER(TAG_FLOAT,0), ((double) (data)) })

/******************
 * String values  *
 ******************/

struct _string_value_t {
    VALUE_HEADER;
    PICC_StringHandle *data;
};

#define PICC_INIT_STRING_VALUE(val, h)					\
    do{									\
	(val)->header = MAKE_HEADER(TAG_STRING,0);			\
	((PICC_StringValue*) (val))->data = (h);			\
    }while(0)

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
    int size;
};

extern void PICC_TupleValue_inv(PICC_TupleValue *tuple);
extern PICC_TupleValue * PICC_free_tuple_value(PICC_TupleValue * tup);
extern void PICC_set_tuple_elements(PICC_Value *tuple, PICC_Value **values);
extern PICC_Value *PICC_get_tuple_element(PICC_Value *val, int index);

/******************
 * Channel values  *
 ******************/

typedef enum {
    PI_CHANNEL =0
}PICC_ChannelKind;

typedef void PICC_ChannelHandle;

struct _channel_value_t {
    VALUE_HEADER ;
    PICC_ChannelHandle *data;
};


extern PICC_ChannelValue *PICC_create_empty_channel_value( PICC_ChannelKind kind );

#define PICC_INIT_CHANNEL_VALUE(val, h)					\
    do{									\
	(val)->header = MAKE_HEADER(TAG_CHANNEL,PI_CHANNEL);		\
	((PICC_ChannelValue*) (val))->data = (h);			\
    }while(0)


extern void PICC_ChannelValue_inv(PICC_ChannelValue *channel);
/* extern PICC_ChannelValue *PICC_free_channel_value( PICC_ChannelValue *channel); */

/**********************************
 * user defined immediate values  *
 **********************************/

struct _user_immediate_value_t {
    VALUE_HEADER ;
    int data;
};

/**********************************
 * user defined managed values  *
 **********************************/

struct _user_managed_value_t {
    VALUE_HEADER ;
    PICC_Handle* data;
};

extern PICC_ChannelValue *PICC_create_pi_channel_value();
extern PICC_ChannelValue *PICC_create_typed_channel_value( PICC_ChannelKind kind );
extern void PICC_ChannelValue_inv(PICC_ChannelValue *channel);

extern void PICC_print_value_infos(PICC_Value * value);


#endif
