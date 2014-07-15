#include <primitives.h>

void corearith_add(PICC_Value* res, PICC_Value* a, PICC_Value* b) {
  PICC_Int_add(res, a, b);
}

void corearith_substract(PICC_Value* res, PICC_Value* a, PICC_Value* b) {
  PICC_Int_substract(res, a, b);
}

void corearith_modulo(PICC_Value* res, PICC_Value* a, PICC_Value* b) {
  PICC_Int_modulo(res, a, b);
}

void corearith_equals(PICC_Value* res, PICC_Value* a, PICC_Value* b) {
  PICC_equals(res, a, b);
}

void corearith_less_than(PICC_Value* res, PICC_Value* a, PICC_Value* b) {
  PICC_Int_less_than(res, a, b);
}

void coreio_print_info(PICC_Value* res, PICC_Value* s) {
  PICC_print_value_infos(s);
}

void coreio_print_str(PICC_Value* res, PICC_Value* s) {
  PICC_print_value(s);
}

void coreio_print_int(PICC_Value* res, PICC_Value* i) {
  PICC_print_value(i);
}

