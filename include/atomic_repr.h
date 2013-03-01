/**
 * @file atomic_repr.h
 * Atomic booleans and integers.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef ATOMIC_REPR_H
#define ATOMIC_REPR_H

#include <stdbool.h>
#include <pthread.h>
#include <atomic.h>
#include <error.h>

/**
 * An atomic boolean encapsulates a boolean value with atomic operations.
 */
struct _PICC_AtomicBoolean {
    bool val;
    pthread_mutex_t lock;
};

/**
 * An atomic integer encapsulates an integer value with atomic operations.
 */
struct _PICC_AtomicInt {
    int val;
    pthread_mutex_t lock;
};

extern PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error);
extern void PICC_free_atomic_bool(PICC_AtomicBoolean *atomic_bool);

/* [TODO] ? pass an integer to the constructor so we don't have
   to do a lock to initialize the value.
 */
extern PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error);
extern void PICC_free_atomic_int(PICC_AtomicInt *atomic_int);

#endif
