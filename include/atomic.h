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
#include <pthread.h>
#include <error.h>

/**
 * An atomic boolean encapsulates a boolean value with atomic operations.
 */
typedef struct _PICC_AtomicBoolean {
    bool val;
    pthread_mutex_t lock;
} PICC_AtomicBoolean;

/**
 * An atomic integer encapsulates an integer value with atomic operations.
 */
typedef struct _PICC_AtomicInt {
    int val;
    pthread_mutex_t lock;
} PICC_AtomicInt;

// Atomic booleans
extern PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error);
extern void PICC_free_atomic_bool(PICC_AtomicBoolean *atomic_bool);
extern bool PICC_atomic_bool_compare_and_swap(PICC_AtomicBoolean *atomic_bool, bool expected_val, bool new_val);
extern bool PICC_atomic_bool_get(PICC_AtomicBoolean *atomic_bool);
extern void PICC_atomic_bool_set(PICC_AtomicBoolean *atomic_bool, bool new_val);

// Atomic integers
extern PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error);
extern void PICC_free_atomic_int(PICC_AtomicInt *atomic_int);
extern int PICC_atomic_int_compare_and_swap(PICC_AtomicInt *atomic_int, int expected_val, int new_val);
extern int PICC_atomic_int_get(PICC_AtomicInt *atomic_int);
extern void PICC_atomic_int_set(PICC_AtomicInt *atomic_int, int new_val);

#endif
