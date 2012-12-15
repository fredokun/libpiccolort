/**
 * @file tools.h
 * Various utility functions.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef TOOLS_H
#define TOOLS_H

#define PICC_ALLOC(var, type, error) \
    type *var = malloc(sizeof(type)); \
    if (var == NULL) { \
        NEW_ERROR(error, ERR_OUT_OF_MEMORY); \
    } else
#endif