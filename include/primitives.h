#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <value.h>

extern void corearith_add(PICC_Value* res, PICC_Value* a, PICC_Value* b);
extern void corearith_substract(PICC_Value* res, PICC_Value* a, PICC_Value* b);
extern void corearith_modulo(PICC_Value* res, PICC_Value* a, PICC_Value* b);
extern void corearith_equals(PICC_Value* res, PICC_Value* a, PICC_Value* b);
extern void corearith_less_than(PICC_Value* res, PICC_Value* a, PICC_Value* b);

extern void coreio_print_info(PICC_Value* res, PICC_Value* s);
extern void coreio_print_str(PICC_Value* res, PICC_Value* s);
extern void coreio_print_int(PICC_Value* res, PICC_Value* i);

#endif
