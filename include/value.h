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

typedef struct _PICC_Value PICC_Clock;
typedef struct _PICC_Value PICC_AtomicBoolean;
typedef struct _PICC_Value PICC_AtomicInt;

extern PICC_Clock *PICC_create_clock();

#endif
