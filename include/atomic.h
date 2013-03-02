/**
 * @file atomic.h
 * Atomic booleans and integers.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdbool.h>

/**
 * An atomic boolean encapsulates a boolean value with atomic operations.
 */
typedef struct _PICC_AtomicBoolean PICC_AtomicBoolean;

/**
 * An atomic integer encapsulates an integer value with atomic operations.
 */
typedef struct _PICC_AtomicInt PICC_AtomicInt;

// Atomic booleans
extern bool PICC_atomic_bool_compare_and_swap(PICC_AtomicBoolean *atomic_bool, bool expected_val, bool new_val);
extern bool PICC_atomic_bool_compare_and_swap_check(PICC_AtomicBoolean *atomic_bool, bool expected_val, bool new_val);
extern bool PICC_atomic_bool_get(PICC_AtomicBoolean *atomic_bool);
// [TODO] Is unconditionally setting an atomic boolean useful?
extern void PICC_atomic_bool_set(PICC_AtomicBoolean *atomic_bool, bool new_val);

// Atomic integers
extern int PICC_atomic_int_compare_and_swap(PICC_AtomicInt *atomic_int, int expected_val, int new_val);
extern bool PICC_atomic_int_compare_and_swap_check(PICC_AtomicInt *atomic_int, int expected_val, int new_val);
extern int PICC_atomic_int_get(PICC_AtomicInt *atomic_int);
// [TODO] Is unconditionally setting an atomic int useful?
extern void PICC_atomic_int_set(PICC_AtomicInt *atomic_int, int new_val);

#endif
