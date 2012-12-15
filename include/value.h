/**
 * @file value.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef VALUE_H
#define VALUE_H

#include <channel.h>
#include <sync.h>
#include <error.h>

typedef enum _PICC_ValueKind {
    PICC_INT_VAL,
    PICC_FLOAT_VAL,
    PICC_STRING_VAL,
    PICC_BOOL_VAL,
    PICC_CHANNEL_VAL,
} PICC_ValueKind;

typedef struct _PICC_Value {
    PICC_ValueKind kind;
    PICC_Mutex lock;
    union {
        int as_int;
        double as_float;
        char *as_string;
        bool as_bool;
        struct _PICC_Channel *as_channel;
    } content;
} PICC_Value;

typedef struct _PICC_Value PICC_AtomicBoolean;
typedef struct _PICC_Value PICC_AtomicInt;

typedef struct _PICC_Clock {
    PICC_AtomicInt *val;
} PICC_Clock;


extern PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error);
extern PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error);
extern PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error);
extern PICC_Clock *PICC_create_clock(PICC_Error *error);

#endif
