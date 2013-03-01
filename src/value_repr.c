/**
 * @file value.c
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Dany SIRIPHOL
 */

#include <value_repr.h>
#include <error.h>
#include <tools.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

void abort_with_message(char * message) {
  fprintf(stderr,message);
  fprintf(stderr,"\nAborting...\n");
  abort();
}

/******************************
 * Immediate values : No value *
 ******************************/

PICC_NoValue *PICC_create_no_value()
{
    PICC_NoValue * val = malloc(sizeof(PICC_NoValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate no value");
    }
    val->header = MAKE_HEADER(TAG_NOVALUE, 0);
    return val;
}

PICC_NoValue *PICC_free_no_value(PICC_NoValue * val)
{
    free(val);
    return NULL;
}

void PICC_no_value_inv(PICC_NoValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    int control = GET_VALUE_CTRL(val->header);
    ASSERT(tag == TAG_NOVALUE )
    ASSERT( control == 0);
}

/******************************
 * Immediate values : boolean *
 ******************************/

PICC_BoolValue *  PICC_create_bool(int boolean) {
    PICC_BoolValue * val = malloc(sizeof(PICC_BoolValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate boolean");
    }
    val->header = MAKE_HEADER(TAG_BOOLEAN, boolean);
    return val;
}

PICC_BoolValue * PICC_free_bool(PICC_BoolValue * val) {
    free(val);
    return NULL;
}

void PICC_bool_inv(PICC_BoolValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    int control = GET_VALUE_CTRL(val->header);
    ASSERT(tag == 2 )
    ASSERT( control == true || control == false);
}

/******************************
 * Immediate values : integer *
 ******************************/

PICC_IntValue *  PICC_create_int(int data) {
  PICC_IntValue * val = malloc(sizeof(PICC_IntValue));
  if (val == NULL) {
    abort_with_message("Cannot allocate integer");
  }
  val->header = MAKE_HEADER(TAG_INTEGER, 0);
  val->data = data;
  return val;
}

PICC_IntValue * free_int(PICC_IntValue * val) {
  free(val);
  return NULL;
}

void PICC_int_inv(PICC_IntValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    ASSERT(tag == TAG_INTEGER )
}

/******************************
 * Immediate values : float *
 ******************************/

PICC_FloatValue *  PICC_create_float(double data) {
    PICC_FloatValue * val = malloc(sizeof(PICC_FloatValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate float");
    }
    val->header = MAKE_HEADER(TAG_FLOAT, 0);
    val->data = data;
    return val;
}

PICC_FloatValue * free_float(PICC_FloatValue * val) {
    free(val);
    return NULL;
}

void PICC_FloatValue_inv(PICC_FloatValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    ASSERT(tag == TAG_FLOAT )
}

/******************
 * Tuples values  *
******************/

PICC_TupleValue * PICC_create_tuple(int arity) {
    PICC_TupleValue * val = malloc(sizeof(PICC_TupleValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate tuple");
    }
    val->header = MAKE_HEADER(TAG_TUPLE, arity);
    val->elements = malloc(sizeof(PICC_Value *) * arity);
    if (val->elements == NULL) {
        abort_with_message("Cannot allocate tuple elements");
    }
    return val;
}

PICC_TupleValue * free_tuple(PICC_TupleValue * tup) {
  free(tup->elements);
  free(tup);
  return NULL;
}

void PICC_TupleValue_inv(PICC_TupleValue *tuple)
{
    ASSERT(tuple != NULL );
    int tag = GET_VALUE_TAG(tuple->header);
    ASSERT(tag == TAG_TUPLE )
}

/******************
 * String values  *
 ******************/

void PICC_init_string_handles(int arity)
{
    #ifdef CONTRACT
        // pre
        ASSERT(arity >= 0);
    #endif

    string_handles = malloc(sizeof(PICC_StringHandle *) * arity);

    #ifdef CONTRACT
        // post
        if(arity == 0)
        {
            ASSERT(string_handles == NULL);
        }
        else
        {
            ASSERT(string_handles != NULL);
        }
    #endif

}

PICC_StringHandle *PICC_create_string_handle(char *string)
{
    #ifdef CONTRACT
        // pre
        ASSERT(string != NULL);
    #endif

    PICC_StringHandle *val = malloc(sizeof( PICC_StringHandle));
    if (val == NULL) {
        abort_with_message("Cannot allocate string handle");
    }
    val->refcount = 1;
    val->data = malloc(sizeof(char)*strlen(string) +1);
    strcpy(val->data,string);

     #ifdef CONTRACT
        PICC_StringHandle_inv(val);
    #endif

    return val;

}

PICC_StringHandle *PICC_free_string_handle(PICC_StringHandle *handle)
{
    free(handle->data);
    free(handle);
    return NULL;
}

void PICC_StringHandle_inv(PICC_StringHandle *handle)
{
    ASSERT(handle != NULL);
    ASSERT(handle->refcount >= 0);
    ASSERT(handle->data != NULL);
}

PICC_StringValue *PICC_create_string_value( char *string )
{
    #ifdef CONTRACT
        // pre
        ASSERT(string != NULL);
    #endif

    PICC_StringValue *val = malloc(sizeof( PICC_StringValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate string");
    }

    PICC_StringHandle *handle = PICC_create_string_handle(string);
    if (handle == NULL) {
        abort_with_message("Cannot allocate string handle");
    }

    val->header = MAKE_HEADER(TAG_STRING, 0);
    val->handle = handle;

    #ifdef CONTRACT

        // inv
        PICC_StringHandle_inv(handle);
        PICC_StringValue_inv(val);
        
    #endif
    return val;
}

PICC_StringValue *PICC_free_string( PICC_StringValue *string )
{
    string->handle->refcount --;
    if( string->handle->refcount == 0 )
        PICC_free_string_handle(string->handle);
    free(string);
    return NULL;
}

void PICC_StringValue_inv(PICC_StringValue *string)
{
    ASSERT(string != NULL);
    int tag = GET_VALUE_TAG(string->header);
    ASSERT(tag == TAG_STRING );
    PICC_StringHandle_inv(string->handle);
}



/******************
 * Channel values  *
 ******************/

PICC_ChannelValue *PICC_create_channel_value( PICC_ChannelKind kind )
{
    PICC_ChannelValue *val = malloc(sizeof( PICC_ChannelValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate channel");
    }
    val->header = MAKE_HEADER(TAG_CHANNEL, kind);
    return val;
}

PICC_ChannelValue *PICC_create_pi_channel_value()
{
    return PICC_create_channel_value(PI_CHANNEL);
}

/* not finished. should use reclaim channel when possible */

PICC_ChannelValue *PICC_free_channel_value( PICC_ChannelValue *channel)
{
    free(channel);
    return NULL;
}

void PICC_ChannelValue_inv(PICC_ChannelValue *channel)
{
    ASSERT(channel != NULL);
    int tag = GET_VALUE_TAG(channel->header);
    int ctrl = GET_VALUE_CTRL(channel->header); 
    ASSERT(tag == TAG_CHANNEL );
    if(ctrl == PI_CHANNEL);
        PICC_Channel_inv(channel->channel);
    
    
}

/**********************************
 * user defined immediate values  *
 **********************************/

PICC_ImmediateValue *PICC_create_immediate_value(int size)
{
    PICC_ImmediateValue *val = malloc(sizeof( PICC_ImmediateValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate immediate");
    }
    val->header = MAKE_HEADER(TAG_USER_DEFINED_IMMEDIATE, 0);
    val->data = malloc(sizeof( int) * size);
    if (val== NULL) {
        abort_with_message("Cannot allocate immediate");
    }
    return val;
}

PICC_ImmediateValue *PICC_free_immediate_value( PICC_ImmediateValue *value)
{
    free(value->data);
    free(value);
    return NULL;
}

void PICC_ImmediateValue_inv(PICC_ImmediateValue *value)
{
    ASSERT(value != NULL);
    int tag = GET_VALUE_TAG(value->header);
    ASSERT(tag ==  TAG_USER_DEFINED_IMMEDIATE );
}


/**********************************
 * user defined managed values  *
 **********************************/

PICC_ManagedValue *PICC_create_managed_value(int size)
{
    PICC_ManagedValue *val = malloc(sizeof( PICC_ManagedValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate managed");
    }
    val->header = MAKE_HEADER(TAG_USER_DEFINED_MANAGED, 0);
    val->data = malloc(sizeof( int) * size);
    if (val == NULL) {
        abort_with_message("Cannot allocate managed");
    }
    return val;
}

PICC_ManagedValue *PICC_free_managed_value( PICC_ManagedValue *value)
{
    free(value->data);
    free(value);
    return NULL;
}

void PICC_ManagedValue_inv(PICC_ManagedValue *value)
{
    ASSERT(value != NULL);
    int tag = GET_VALUE_TAG(value->header);
    ASSERT(tag ==  TAG_USER_DEFINED_MANAGED );
}

/**** Example of dispatch function *****/

void print_value_infos(PICC_Value * value) {
  PICC_TagValue tag = GET_VALUE_TAG(value->header);
  int ctrl = GET_VALUE_CTRL(value->header);

  printf("------------\n");
  printf("tag = %d\n", GET_VALUE_TAG(value->header));
  printf("ctrl = %d\n", GET_VALUE_CTRL(value->header));

  switch(tag) {
  case TAG_INTEGER: {
    printf("Type: integer\n");
    PICC_IntValue *int_value = (PICC_IntValue *) value;
    printf("Value = %d\n", int_value->data);
    break;
  }
  case TAG_BOOLEAN: {
    printf("Type: boolean\n");
    if (ctrl == 0) {
      printf("Value = False\n");
    } else if(ctrl == 1) {
      printf("Value = True\n");
    } else {
      abort_with_message("Unknown boolean value");
    }
    break;
  }
  case TAG_TUPLE: {
    printf("Type: tuple\n");
    PICC_TupleValue *tup = (PICC_TupleValue *) value;
    for(int i=0;i<ctrl;i++) {
      printf("%d-th element>>>>>>>>>\n",i);
      print_value_infos(tup->elements[i]);
      printf("<<<<<<<<<<<\n");
    }
    break;
  }
  default:
    abort_with_message("unknown tag");

  }
}

/**
 * Creates a new clock.
 *
 * @param error Error stack
 * @return Created clock
 */
PICC_Clock *PICC_create_clock(PICC_Error *error)
{
/*
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
*/
return NULL;
}

PICC_AtomicBoolean *PICC_create_atomic_boolean(PICC_Error *error)
{
/*
    PICC_AtomicBoolean *abool = PICC_create_value(PICC_BOOL_VAL, error);
    if (abool != NULL) {
        abool->content.as_bool = false;
    }
    return abool;
*/
return NULL;
}
