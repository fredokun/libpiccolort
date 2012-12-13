/**
 * @file error.c
 * Provides several functions to handle errors.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "error.h"

/**
 * Initializes a new error with a given error ID, file and line.
 *
 * @param error New error to be returned by the caller.
 * @param file File where the error occured.
 * @param line Line in <file> where the error occured.
 * @return Initialized error.
 */
void PICC_init_error(PICC_Error *error, PICC_ErrorId id, const char *file, int line)
{
    if (error != NULL) {
        error->id = id;
        error->file = malloc(strlen(file) * sizeof(char));
        if (error->file == NULL) {
            perror("Unable to create a new PICC_Error");
            exit(EXIT_FAILURE);
        }
        strncpy(error->file, file, strlen(file));
        error->line = line;
        error->prev = NULL;
    }
}

/**
 * Adds a new error to the error stack by appending the previous error to the
 * new one.
 *
 * @param error New error to be returned by the caller.
 * @param prev_error Previous error occured.
 * @param file File where the error occured.
 * @param line Line in <file> where the error occured.
 * @return New error linked to previous error.
 */
void PICC_add_error(PICC_Error *error, PICC_Error prev_error, PICC_ErrorId id, const char *file, int line)
{
    if (error != NULL) {
        PICC_init_error(error, id, file, line);
        error->prev = PICC_copy_error(prev_error);
    }
}

/**
 * Copies a given error to a new allocated memory location.
 *
 * This is used to ensure the previous error in an error stack are not
 * automatically freed.
 *
 * @param error Error to be copied.
 * @param Copy of <error>.
 */
PICC_Error *PICC_copy_error(const PICC_Error error)
{
    PICC_Error *copy = malloc(sizeof(PICC_Error));
    if (copy == NULL) {
        fprintf(stderr, "Error: Can't create a new PICC_Error.\n");
        exit(EXIT_FAILURE);
    }
    PICC_init_error(copy, error.id, error.file, error.line);
    copy->prev = error.prev;
    return copy;
}

/**
 * Prints the given error stack on the standard error output and quits the
 * program with failure return code.
 *
 * @param error Error stack
 * @param file File where the crash occured (for DEBUG)
 * @param fct Function where the crash occured (for DEBUG)
 */
void PICC_crash(PICC_Error *error, const char *file, const char *fct)
{
    PICC_print_error(error, file, fct);
    exit(EXIT_FAILURE);
}

/**
 * Prints the given error stack on the standard error output. The sub-errors are
 * freed after being printed.
 *
 * @param error Error stack
 * @param file File where the print occured (for DEBUG)
 * @param fct Function where the print occured (for DEBUG)
 */
void PICC_print_error(PICC_Error *error, const char *file, const char *fct)
{
    if (error->id > 0) {
        fprintf(stderr, "%s: In function '%s':\n", file, fct);
        fprintf(stderr, "%s:%d: error: %s\n", error->file, error->line, PICC_get_error_message(error->id));

        if (error->prev != NULL) {
            PICC_Error *prev = error->prev;
            while (prev != NULL) {
                fprintf(stderr, "%s:%d: error: %s\n", prev->file, prev->line, PICC_get_error_message(prev->id));
                prev = prev->prev;
            }

            PICC_free_error(error->prev);
            error->prev = NULL;
        }
    }
}

/**
 * Forwards an error by copying all the value of the previous error in the
 * given error.
 *
 * @param error Current error.
 * @param prev_error Previous error.
 */
void PICC_forward_error(PICC_Error *error, PICC_Error prev_error)
{
    if (error != NULL) {
        PICC_init_error(error, prev_error.id, prev_error.file, prev_error.line);
        error->prev = prev_error.prev;
    }
}

/**
 * Returns the error message corresponding to the given error ID.
 *
 * @param id Error identifier
 * @return Error message
 */
const char *PICC_get_error_message(PICC_ErrorId id)
{
    return PICC_error_messages[id];
}

/**
 * Frees the memory used by an error stack.
 *
 * @param error Error stack.
 */
void PICC_free_error(PICC_Error *error)
{
    if (error->file != NULL)
        free(error->file);
    if (error->prev != NULL)
        PICC_free_error(error->prev);
    free(error);
}
