/**
 * @file symbols.h
 * Runtime symbols and constants
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef SYMBOLS_H
#define SYMBOLS_H

static const int PICC_FUEL_INIT = 358;

static const int PICC_INVALID_PC = -1;

/**
 * The label type. Used by the pi-threads to point to a place where to
 * start it's procedure.
 */
typedef int PICC_Label;

#endif
