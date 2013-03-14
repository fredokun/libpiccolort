/**
 * @file knownset.h
 * Data structures for knownsets.
 *
 * This project is released under MIT License.
 *
 * @author Loïc Girault
 */

#ifndef KNOWNSET_H
#define KNOWNSET_H

#include <stdbool.h>
#include <error.h>

#define NOT_INDEXED -1

/* #define IS_CHANNEL(header) (GET_VALUE_CTRL(value->header) == TAG_CHANNEL) */
/* #define IS_STRING(header) (GET_VALUE_CTRL(value->header) == TAG_STRING) */

/**
 * The known state of a channel
 */
enum _PICC_KnownState {
    PICC_UNKNOWN,
    PICC_KNOWN,
    PICC_FORGET
};

/**
 * Abstraction of a managed value for the KnownSet
 */
typedef struct _PICC_KnownValue PICC_KnownValue;

typedef struct _PICC_KnownSet PICC_KnownSet;

/**
 * The known state of a managed value
 */
typedef enum _PICC_KnownState PICC_KnownState;

extern PICC_KnownSet *PICC_create_known_set(int init_max_size, PICC_Error* e);
extern PICC_KnownSet *PICC_create_empty_known_set();

extern void PICC_free_known_set(PICC_KnownSet *s);

extern bool PICC_known_set_add(PICC_KnownSet *s, PICC_KnownValue *val);
extern int PICC_known_set_size(PICC_KnownSet *s);

extern PICC_KnownSet *PICC_known_know(PICC_KnownSet *ks);
extern PICC_KnownSet *PICC_known_forget(PICC_KnownSet *ks);

extern void PICC_known_forget_to_unknown_gen(PICC_KnownSet *ks, PICC_KnownValue *val);
extern void PICC_known_forget_all(PICC_KnownSet *ks);

extern bool PICC_known_register(PICC_KnownSet *ks, PICC_KnownValue *val);
extern void PICC_known_set_inv(PICC_KnownSet *set);
#endif
