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
#include <channel_repr.h>
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

PICC_Value* PICC_free_value(PICC_Value *v){

    switch(GET_VALUE_TAG(v->header)){
	/*Do nothing */
    case TAG_RESERVED:
    case TAG_NOVALUE:
    case TAG_BOOLEAN:
	return NULL;
	
    case TAG_INTEGER:
	return (PICC_Value*) PICC_free_int((PICC_IntValue*) v);

    case TAG_STRING:
	return (PICC_Value*) PICC_free_string((PICC_StringValue*) v);

    /* case TAG_CHANNEL: */
    /* 	return (PICC_Value*) PICC_free_channel_value((PICC_ChannelValue*) v); */
	
	/*TODO*/
    case TAG_FLOAT: 
    case TAG_TUPLE:
    case TAG_USER_DEFINED_IMMEDIATE:	
    case TAG_USER_DEFINED_MANAGED:
	return NULL;
    default:
	return NULL;
    }
}

/******************************
 * Immediate values : No value *
 ******************************/

static PICC_NoValue picc_novalue = {MAKE_HEADER(TAG_NOVALUE, 0)};

PICC_Value *PICC_create_no_value()
{
   PICC_Value * val = (PICC_Value*) &picc_novalue;
    
    #ifdef CONTRACT_POST_INV
   PICC_NoValue_inv((PICC_NoValue*)val);
    #endif

    return val;
}

void PICC_NoValue_inv(PICC_NoValue * val)
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

static PICC_BoolValue picc_true = {MAKE_HEADER(TAG_BOOLEAN, true)};
static PICC_BoolValue picc_false = {MAKE_HEADER(TAG_BOOLEAN, false)};


PICC_Value *  PICC_create_bool_value(bool boolean) {
    PICC_BoolValue * val;
    if (boolean){
	val = &picc_true;
    }
    else{
	val = &picc_false;
    }

#ifdef CONTRACT_POST_INV
    PICC_BoolValue_inv(val);
#endif

#ifdef CONTRACT_POST
    //post
    ASSERT(GET_VALUE_CTRL(val->header) == boolean );
#endif

    return (PICC_Value*) val;
}

extern bool PICC_bool_of_bool_value(PICC_Value *val){
    
    if(! IS_BOOLEAN(val)){
	abort_with_message("PICC_bool_of_bool_value - Arg was not a BoolValue");
    }
    
    return (PICC_BoolValue*) val == &picc_true;
    
}


void PICC_BoolValue_inv(PICC_BoolValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    int control = GET_VALUE_CTRL(val->header);
    ASSERT(tag == 2 );
    ASSERT( control == true || control == false);
}

// boolean primitives
void PICC_Bool_and( PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_BoolValue * bv1= (PICC_BoolValue*) v1;
    PICC_BoolValue * bv2= (PICC_BoolValue*) v2;
   
    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif
	
    PICC_free_value(res);
    
    int bres = GET_VALUE_CTRL(bv1->header) & GET_VALUE_CTRL(bv2->header);
    
    if(bres) res = (PICC_Value*) &picc_true;
    else res = (PICC_Value*) &picc_false;
       
    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(GET_VALUE_CTRL(val->header) == (control & boolean) );
    #endif
}

void PICC_Bool_or ( PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_BoolValue * bv1= (PICC_BoolValue*) v1;
    PICC_BoolValue * bv2= (PICC_BoolValue*) v2;
   
    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif
	
    PICC_free_value(res);
    
    int bres = GET_VALUE_CTRL(bv1->header) | GET_VALUE_CTRL(bv2->header);
    
    if(bres) res = (PICC_Value*) &picc_true;
    else res = (PICC_Value*) &picc_false;
       
    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(GET_VALUE_CTRL(val->header) == (control & boolean) );
    #endif
}


void PICC_Bool_xor( PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_BoolValue * bv1= (PICC_BoolValue*) v1;
    PICC_BoolValue * bv2= (PICC_BoolValue*) v2;
   
    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif
	
    PICC_free_value(res);
    
    int bres = GET_VALUE_CTRL(bv1->header) ^ GET_VALUE_CTRL(bv2->header);
    
    if(bres) res = (PICC_Value*) &picc_true;
    else res = (PICC_Value*) &picc_false;
       
    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv1);
        PICC_BoolValue_inv(bv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(GET_VALUE_CTRL(val->header) == (control & boolean) );
    #endif
}


void PICC_Bool_not( PICC_Value *res, PICC_Value *v)
{
    PICC_BoolValue * bv= (PICC_BoolValue*) v;
    
    #ifdef CONTRACT_PRE_INV
        PICC_BoolValue_inv(bv);
    #endif
	
    PICC_free_value(res);
    
    int bres = ! GET_VALUE_CTRL(bv->header);
    
    if(bres) res = (PICC_Value*)&picc_true;
    else res = (PICC_Value*)&picc_false;
       
    #ifdef CONTRACT_POST_INV
        PICC_BoolValue_inv(bv);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(GET_VALUE_CTRL(val->header) == (control & boolean) );
    #endif
}

/******************************
 * Immediate values : integer *
 ******************************/

PICC_Value * PICC_create_int_value(int data) {

    PICC_IntValue * val = malloc(sizeof(PICC_IntValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate integer");
    }
    val->header = MAKE_HEADER(TAG_INTEGER, 0);
    val->data = data;

    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(val);
    #endif

    return (PICC_Value *) val;
}

PICC_IntValue * PICC_free_int(PICC_IntValue * val) {
    free(val);
    val = NULL;
    return val;
}

void PICC_IntValue_inv(PICC_IntValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    ASSERT(tag == TAG_INTEGER )
}

void PICC_Int_add (PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

#ifdef CONTRACT_PRE_INV
    PICC_IntValue_inv(iv1);
    PICC_IntValue_inv(iv2);
#endif

#ifdef CONTRACT_POST
    //capture
    //int data_at_pre = val->data;
#endif

    int r = iv1->data + iv2->data;
    
    if(IS_INT(res)){
	((PICC_IntValue*) res)->data = r;
    }
    else{
	PICC_free_value(res);
	res = PICC_create_int_value(r);
    }
    
    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(val->data == (data_at_pre + value) );
    #endif
}

void PICC_Int_multiply (PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

#ifdef CONTRACT_PRE_INV
    PICC_IntValue_inv(iv1);
    PICC_IntValue_inv(iv2);
#endif

#ifdef CONTRACT_POST
    //capture
    //int data_at_pre = val->data;
#endif

    int r = iv1->data * iv2->data;
    
    if(IS_INT(res)){
	((PICC_IntValue*) res)->data = r;
    }
    else{
	PICC_free_value(res);
	res = PICC_create_int_value(r);
    }
    
    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(val->data == (data_at_pre + value) );
    #endif
}

void PICC_Int_divide (PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

#ifdef CONTRACT_PRE_INV
    PICC_IntValue_inv(iv1);
    PICC_IntValue_inv(iv2);
#endif

#ifdef CONTRACT_POST
    //capture
    //int data_at_pre = val->data;
#endif

    int r = iv1->data / iv2->data;
    
    if(IS_INT(res)){
	((PICC_IntValue*) res)->data = r;
    }
    else{
	PICC_free_value(res);
	res = PICC_create_int_value(r);
    }
    
    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(val->data == (data_at_pre + value) );
    #endif
}

void PICC_Int_substract(PICC_Value *res, PICC_Value *v1, PICC_Value *v2)
{
    PICC_IntValue * iv1 = (PICC_IntValue*) v1;
    PICC_IntValue * iv2 = (PICC_IntValue*) v2;

#ifdef CONTRACT_PRE_INV
    PICC_IntValue_inv(iv1);
    PICC_IntValue_inv(iv2);
#endif

#ifdef CONTRACT_POST
    //capture
    //int data_at_pre = val->data;
#endif

    int r = iv1->data - iv2->data;
    
    if(IS_INT(res)){
	((PICC_IntValue*) res)->data = r;
    }
    else{
	PICC_free_value(res);
	res = PICC_create_int_value(r);
    }
    
    #ifdef CONTRACT_POST_INV
        PICC_IntValue_inv(iv1);
        PICC_IntValue_inv(iv2);
    #endif

    #ifdef CONTRACT_POST
        //post
        //ASSERT(val->data == (data_at_pre + value) );
    #endif
}

/******************************
 * Immediate values : float *
 ******************************/

//TODO

/******************
 * Tuples values  *
******************/

/* PICC_TupleValue * PICC_create_tuple_value(int size) { */

/*     #ifdef CONTRACT_PRE */
/*         //pre */
/* 		ASSERT(size >= 0); */
/*     #endif */

/*     PICC_TupleValue * val = malloc(sizeof(PICC_TupleValue)); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate tuple"); */
/*     } */
/*     val->header = MAKE_HEADER(TAG_TUPLE, size); */
/*     val->elements = malloc(sizeof(PICC_Value *) * size); */
/*     if (val->elements == NULL) { */
/*         abort_with_message("Cannot allocate tuple elements"); */
/*     } */
    
/*     #ifdef CONTRACT_POST_INV */
/*         PICC_TupleValue_inv(val); */
/*     #endif */

/*     return val; */
/* } */

/* PICC_TupleValue * free_tuple(PICC_TupleValue * tup) { */
/*   free(tup->elements); */
/*   free(tup); */
/*   return NULL; */
/* } */

/* void PICC_TupleValue_inv(PICC_TupleValue *tuple) */
/* { */
/*     ASSERT(tuple != NULL ); */
/*     int tag = GET_VALUE_TAG(tuple->header); */
/*     ASSERT(tag == TAG_TUPLE ) */
/* } */

/******************
 * String values  *
 ******************/

PICC_StringHandle *PICC_create_string_handle(char *string)
{
#ifdef CONTRACT_PRE
    // pre
    ASSERT(string != NULL);
#endif

    PICC_StringHandle *val = malloc(sizeof(PICC_StringHandle));
    if (val == NULL) {
        abort_with_message("Cannot allocate string handle");
    }
    val->refcount = PICC_create_atomic_int(NULL);
    PICC_atomic_int_set(val->refcount, 1);
    val->data = malloc(sizeof(char)*strlen(string) +1);
    strcpy(val->data, string);

#ifdef CONTRACT_POST_INV
    PICC_StringHandle_inv(val);
#endif

    return val;

}

PICC_StringHandle *PICC_free_string_handle(PICC_StringHandle *handle)
{
    free(handle->data);
    PICC_free_atomic_int(handle->refcount);
    free(handle);
    return NULL;
}

void PICC_StringHandle_inv(PICC_StringHandle *handle)
{
    ASSERT(handle != NULL);
    ASSERT(handle->refcount >= 0);
    ASSERT(handle->data != NULL);
}


static PICC_StringValue *PICC_create_empty_string_value(){
    PICC_StringValue *val = malloc(sizeof(PICC_StringValue));
    val->header = MAKE_HEADER(TAG_STRING, 0);
    return val;
}

PICC_Value *PICC_create_string_value( char *string )
{
#ifdef CONTRACT_PRE
    // pre
    ASSERT(string != NULL);
#endif

    PICC_StringValue *val = PICC_create_empty_string_value();
    
    if (val == NULL) {
        abort_with_message("Cannot allocate string");
    }

    PICC_StringHandle *handle = PICC_create_string_handle(string);
    if (handle == NULL) {
        abort_with_message("Cannot allocate string handle");
    }

    val->handle = handle;

#ifdef CONTRACT_POST_INV
    PICC_StringHandle_inv(handle);
    PICC_StringValue_inv(val);
#endif

    return (PICC_Value*)val;
}

PICC_StringValue *PICC_free_string( PICC_StringValue *string )
{
    if(string == NULL){
	abort_with_message("PICC_free_string - Cannot free a NULL pointer");
    }

    PICC_AtomicInt *at_int=string->handle->refcount;
    
    int i;
    do{
	i = PICC_atomic_int_get(at_int);
    }while(! PICC_atomic_int_bool_compare_and_swap(at_int, i, i-1));
    
    
    if (PICC_atomic_int_get(at_int) == 0 ){ //!\ same test in copy, if = 0 -> failure
        PICC_free_string_handle(string->handle);
    }
    free(string);
    string = NULL;
    return string;
}

bool PICC_copy_string(PICC_Value *to, PICC_StringValue* from){
    PICC_AtomicInt *at_int=from->handle->refcount;
    int i;
    do{
	i = PICC_atomic_int_get(at_int);
	if(i == 0){
	    return false;
	}
    }while(! PICC_atomic_int_bool_compare_and_swap(at_int, i, i+1));
    
    PICC_StringValue* strto = (PICC_StringValue*) to;
    if(IS_STRING(to)){
	strto->handle = from->handle;
    }
    else{
	PICC_free_value(to);
	strto = PICC_create_empty_string_value();
	strto->handle = from->handle;
    }
    return true;
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

PICC_ChannelValue *PICC_create_empty_channel_value( PICC_ChannelKind kind )
{
    PICC_ChannelValue *val = malloc(sizeof( PICC_ChannelValue));
    if (val == NULL) {
        abort_with_message("Cannot allocate channel");
    }
    val->header = MAKE_HEADER(TAG_CHANNEL, kind);

    #ifdef CONTRACT_POST_INV
        PICC_ChannelValue_inv(val);
    #endif

    return val;
}

/* PICC_Value *PICC_create_pi_channel_value() */
/* { */
/*     return (PICC_Value*) PICC_create_channel_value(PI_CHANNEL); */
/* } */

PICC_Value *PICC_create_channel_value(PICC_Channel* channel)
{
    PICC_ChannelValue *val = PICC_create_empty_channel_value(PI_CHANNEL);
    val->channel = (void*)channel;
    return (PICC_Value*) val;
}

PICC_Channel *PICC_channel_of_channel_value(PICC_Value* channel)
{
    if(!IS_CHANNEL(channel)){
	abort_with_message("PICC_channel_of_channel_value - value is not a channel");
    }
    
    return (PICC_Channel*) ((PICC_ChannelValue*) channel)->channel;
    
}

/* not finished. should use reclaim channel when possible */

PICC_ChannelValue *PICC_free_channel_value( PICC_ChannelValue *channel)
{
    free(channel);
    return NULL;
}

void PICC_channel_value_acquire(PICC_Value* channel){
    if(!IS_CHANNEL(channel)){
	abort_with_message("PICC_channel_value_acquire - value is not a channel");
    }
    
    PICC_Channel *c = (PICC_Channel*) ((PICC_ChannelValue*) channel)->channel;
    
    PICC_acquire(&c->lock);
}

int PICC_channel_value_global_rc(PICC_Value* channel){
    if(!IS_CHANNEL(channel)){
	abort_with_message("PICC_channel_value_global_rc - value is not a channel");
    }
    
    PICC_Channel *c = (PICC_Channel*) ((PICC_ChannelValue*) channel)->channel;
    
    return c->global_rc;
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

/* PICC_ImmediateValue *PICC_create_immediate_value(int size) */
/* { */
/*     #ifdef CONTRACT_PRE */
/*         //pre */
/* 		ASSERT(size >= 0); */
/*     #endif */

/*     PICC_ImmediateValue *val = malloc(sizeof( PICC_ImmediateValue)); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate immediate"); */
/*     } */
/*     val->header = MAKE_HEADER(TAG_USER_DEFINED_IMMEDIATE, 0); */
/*     val->data = malloc(sizeof( int) * size); */
/*     if (val== NULL) { */
/*         abort_with_message("Cannot allocate immediate"); */
/*     } */

/*     #ifdef CONTRACT_POST_INV */
/*         PICC_ImmediateValue_inv(val); */
/*     #endif */

/*     return val; */
/* } */

/* PICC_ImmediateValue *PICC_free_immediate_value( PICC_ImmediateValue *value) */
/* { */
/*     free(value->data); */
/*     free(value); */
/*     return NULL; */
/* } */

/* void PICC_ImmediateValue_inv(PICC_ImmediateValue *value) */
/* { */
/*     ASSERT(value != NULL); */
/*     int tag = GET_VALUE_TAG(value->header); */
/*     ASSERT(tag ==  TAG_USER_DEFINED_IMMEDIATE ); */
/* } */


/**********************************
 * user defined managed values  *
 **********************************/

/* PICC_ManagedValue *PICC_create_managed_value(int size) */
/* { */

/*     #ifdef CONTRACT_PRE */
/*         //pre */
/* 		ASSERT(size >= 0); */
/*     #endif */

/*     PICC_ManagedValue *val = malloc(sizeof( PICC_ManagedValue)); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate managed"); */
/*     } */
/*     val->header = MAKE_HEADER(TAG_USER_DEFINED_MANAGED, 0); */
/*     val->data = malloc(sizeof( int) * size); */
/*     if (val == NULL) { */
/*         abort_with_message("Cannot allocate managed"); */
/*     } */

/*     #ifdef CONTRACT_POST_INV */
/*         PICC_ManagedValue_inv(val); */
/*     #endif */
    
/*     return val; */
/* } */

/* PICC_ManagedValue *PICC_free_managed_value( PICC_ManagedValue *value) */
/* { */
/*     free(value->data); */
/*     free(value); */
/*     return NULL; */
/* } */

/* void PICC_ManagedValue_inv(PICC_ManagedValue *value) */
/* { */
/*     ASSERT(value != NULL); */
/*     int tag = GET_VALUE_TAG(value->header); */
/*     ASSERT(tag ==  TAG_USER_DEFINED_MANAGED ); */
/* } */

/* void PICC_copy_value(PICC_Value *to, PICC_Value *from){ */
  
/*    if(to!=NULL) */
/*       PICC_free(to); */
   
   
/* } */

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
  /* case TAG_TUPLE: { */
  /*   printf("Type: tuple\n"); */
  /*   PICC_TupleValue *tup = (PICC_TupleValue *) value; */
  /*   for(int i=0;i<ctrl;i++) { */
  /*     printf("%d-th element>>>>>>>>>\n",i); */
  /*     print_value_infos(tup->elements[i]); */
  /*     printf("<<<<<<<<<<<\n"); */
  /*   } */
  /*   break; */
  /* } */
  case TAG_STRING:
      printf("%s\n", ((PICC_StringValue *)value)->handle->data );
      break;
  default:
    abort_with_message("unknown tag");

  }
}
