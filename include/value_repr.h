/**
 * @file value_repr.h
 * Values exchanged through the program.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 * @author Maxence WO
 */

#ifndef VALUE_REPR_H
#define VALUE_REPR_H

#include <value.h>
#include <channel.h>
#include <atomic.h>
#include <concurrent.h>
#include <error.h>

/**
 * The types a value may have
 */
enum _PICC_ValueKind {
    PICC_INT_VAL, /**< integer */
    PICC_FLOAT_VAL, /**< single float */
    PICC_STRING_VAL, /**< string (char *) */
    PICC_BOOL_VAL, /**< booolean */
    PICC_CHANNEL_VAL, /**< channel (@see channel.h) */
};

/**
 * A type to represent an arbitrary value
 */
struct _PICC_Value {
    /**@{*/
    PICC_ValueKind kind; /**< The real type of the value */
    PICC_Lock lock; /** A lock that may be used to block the value for
                        concurent accesses */
    /**@}*/
    /**
     * The possible types a value can have.
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
};

/**
 * A type to represent PiThread clocks
 */
struct _PICC_Clock {
    /**@{*/
    PICC_AtomicInt *val; /** Contains the timestamp when the clock has
                           * been stopped. TODO a function that puts a
                           * timestamp in a clock */
    /**@{*/
};

extern PICC_Value *PICC_create_value(PICC_ValueKind type, PICC_Error *error);
extern PICC_Clock *PICC_create_clock(PICC_Error *error);
extern void PICC_reclaim_clock(PICC_Clock *clock);

extern PICC_Value *PICC_create_value_int(int i, PICC_Error *error);

#endif
