/**
 * @file knownset.h
 * Data structures for knownsets.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
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

typedef struct _PICC_Known PICC_Known;
typedef struct _PICC_Handle PICC_Handle;

/**
 * The known state of a channel
 */
typedef enum _PICC_KnownState PICC_KnownState;

extern PICC_Known *PICC_create_empty_known();
extern PICC_Known *PICC_create_known(int size, int type);

extern void PICC_free_known(PICC_Known *s);

extern bool PICC_known_add(PICC_Known *s, PICC_Handle *elem);
extern int PICC_known_size(PICC_Known *s);

extern PICC_Known *PICC_known_know(PICC_Known *ks);
extern PICC_Known *PICC_known_forget(PICC_Known *ks);

extern void PICC_known_forget_to_unknown_gen(PICC_Known *ks, PICC_Handle *val);
extern void PICC_known_forget_all(PICC_Known *ks);

extern bool PICC_known_register_gen(PICC_Known *ks, PICC_Handle *val);
extern void PICC_Known_Inv(PICC_Known *set);
#endif
