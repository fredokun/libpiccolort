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

/**
 * The known state of a channel
 */
enum _PICC_KnownState {
    PICC_UNKNOWN, /**< TODO see spec */
    PICC_KNOWN, /**< TODO see spec */
    PICC_FORGET /**< TODO see spec */
};


typedef struct _PICC_Value GEN_VALUE;
typedef struct _PICC_PiResource PICC_PiResource;
typedef struct _PICC_KnownSet PICC_KnownSet;


/**
 * The known state of a channel
 */
typedef enum _PICC_KnownState PICC_KnownState;

extern PICC_KnownSet *PICC_create_empty_known_set();
extern PICC_KnownSet *PICC_create_known_set(int size, PICC_Error* error);

extern void PICC_free_known_set(PICC_KnownSet *s);

extern bool PICC_known_set_add(PICC_KnownSet *s, GEN_VALUE *elem);
extern int PICC_known_set_size(PICC_KnownSet *s);

extern PICC_KnownSet *PICC_knowns_set_knows(PICC_KnownSet *ks);
extern PICC_KnownSet *PICC_knowns_set_forget(PICC_KnownSet *ks);

extern void PICC_knowns_set_forget_to_unknown_gen(PICC_KnownSet *ks, GEN_VALUE *val);
extern void PICC_knowns_set_forget_all(PICC_KnownSet *ks);

extern bool PICC_knowns_register_gen(PICC_KnownSet *ks, GEN_VALUE *val);
extern void PICC_KnownSet_Inv(PICC_KnownSet *set);
#endif
