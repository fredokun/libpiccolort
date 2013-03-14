/**
 * @file knownset.h
 * Data structures for known sets.
 *
 * This project is released under MIT License.
 *
 * @author Loïc Girault
 * @author Maxence Wo
 * @author Mickaël Menu
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

// life cycle
extern PICC_KnownSet *PICC_create_knownset(int init_max_size, PICC_Error* e);
extern PICC_KnownSet *PICC_create_empty_knownset();
extern void PICC_free_knownset(PICC_KnownSet *s);

extern bool PICC_knownset_register(PICC_KnownSet *ks, PICC_KnownValue *val);
extern bool PICC_knownset_add(PICC_KnownSet *s, PICC_KnownValue *val);
extern int PICC_knownset_size(PICC_KnownSet *s);

// subsets creation
extern PICC_KnownSet *PICC_knownset_known(PICC_KnownSet *ks);
extern PICC_KnownSet *PICC_knownset_forget(PICC_KnownSet *ks);

// state changes
extern void PICC_knownset_forget_to_unknown(PICC_KnownSet *ks, PICC_KnownValue *val);
extern void PICC_knownset_forget_all(PICC_KnownSet *ks);

#endif
