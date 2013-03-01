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
#include <atomic_repr.h>
#include <tools.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
s

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
/******************************/

typedef struct _int_value_t {
  VALUE_HEADER ;
  int data ;
} PICC_IntValue;

#define MAKE_INT_VALUE(data) ((PICC_IntValue)  { MAKE_HEADER(TAG_INTEGER,0), ((int) (data)) })

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
/******************************/

typedef struct _float_value_t {
  VALUE_HEADER ;
  double data ;
} PICC_FloatValue ;

#define MAKE_FLOAT_VALUE(data) ((PICC_FloatValue)  { MAKE_HEADER(TAG_FLOAT,0), ((int) (data)) })

PICC_FloatValue *  PICC_create_float(double float) {
  PICC_FloatValue * val = malloc(sizeof(PICC_FloatValue));
  if (val == NULL) {
    abort_with_message("Cannot allocate float");
  }
  val->header = MAKE_HEADER(TAG_FLOAT, 0);
  val->data = data;
  return val;
}

PICC_FloatValue * free_int(PICC_FloatValue * val) {
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
/******************/

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

void PICC_TupleValue_inv(PICC_FloatValue * val)
{
    ASSERT(val != NULL );
    int tag = GET_VALUE_TAG(val->header);
    ASSERT(tag == TAG_TUPLE )
}

/**** Example of dispatch function *****/

void print_value_infos(value_t * value) {
  tag_t tag = GET_VALUE_TAG(value->header);
  int ctrl = GET_VALUE_CTRL(value->header);

  printf("------------\n");
  printf("tag = %d\n", GET_VALUE_TAG(value->header));
  printf("ctrl = %d\n", GET_VALUE_CTRL(value->header));

  switch(tag) {
  case TAG_INTEGER: {
    printf("Type: integer\n");
    int_value_t * int_value = (int_value_t *) value;
    printf("Value = %d\n", int_value->data);
    break;
  }
  case TAG_BOOLEAN: {
    printf("Type: boolean\n");
    bool_value_t * bool_value = (bool_value_t *) value;
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
    tuple_value_t * tup = (tuple_value_t *) value;
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
 * Creates a new value of given type.
 *
 * @param type Type of the value
 * @param error Error stack
 * @return Created value
 */

PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error)
{
    PICC_ALLOC(value, PICC_Value, error) {
        value->kind = type;
        PICC_init_lock(&(value->lock));
    }
    return value;
}

PICC_Value *PICC_create_value_int(int i, PICC_Error *error)
{
    PICC_Value* value = PICC_create_value(PICC_INT_VAL,error);
    value->content.as_int = i;
    return value;
}
