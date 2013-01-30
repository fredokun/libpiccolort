/**
 * @file value.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#ifndef VALUE_H
#define VALUE_H

#include <channel.h>
#include <sync.h>
#include <error.h>

/**
 * The types a value may have
 */
typedef enum _PICC_ValueKind {
    PICC_INT_VAL, /**< integer */
    PICC_FLOAT_VAL, /**< single float */
    PICC_STRING_VAL, /**< string (char *) */
    PICC_BOOL_VAL, /**< booolean */
    PICC_CHANNEL_VAL, /**< channel (@see channel.h) */
} PICC_ValueKind;

/**
 * A type to represent an arbitrary value
 */
typedef struct _PICC_Value {
    /**@{*/
    PICC_ValueKind kind; /**< The real type of the value */
    PICC_Mutex lock; /** A lock that may be used tu block the value for
                        concurent accesses */
    /**@}*/
    /**
     * @name The possible types a value can have.
     * Only the value for the type that corresponds to the @param kind
     * matters.
     */
    union {
        /**@{*/
        int as_int; /**< The integer of the value */
        double as_float; /**< The float of the value */
        char *as_string; /**< The string of the value */
        bool as_bool; /**< The boolean of the value */
        struct _PICC_Channel *as_channel; /**< The channel of the value */
        /**@}*/
    } content;
} PICC_Value;

/**
 * An atomic boolean is a PICC_Value that has PICC_BOOL_VAL in kind.
 * To acquire and release an atomic boolean, on it's lock of use the
 * PICC_acquire and PICC_release functions that are defined in sync.h
 */
typedef struct _PICC_Value PICC_AtomicBoolean;

/**
 * An atomic integer is a PICC_Value that has PICC_INT_VAL in kind.
 * To acquire and release an atomic integer, on it's lock of use the
 * PICC_acquire and PICC_release functions that are defined in sync.h
 */
typedef struct _PICC_Value PICC_AtomicInt;

/**
 * A type to represent PiThread clocks
 */
typedef struct _PICC_Clock {
    /**@{*/
    PICC_AtomicInt *val; /** Contains the timestamp when the clock has
                           * been stopped. TODO a function that puts a
                           * timestamp in a clock */
    /**@{*/
} PICC_Clock;


extern PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error);
extern PICC_AtomicBoolean *PICC_create_atomic_bool(PICC_Error *error);
extern PICC_AtomicInt *PICC_create_atomic_int(PICC_Error *error);
extern PICC_Clock *PICC_create_clock(PICC_Error *error);

#endif
