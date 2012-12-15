/**
 * @file error.h
 * Provides several macros and functions to handle errors.
 *
 * Senseless example of use:
 * int main() {
 *     ALLOC_ERROR(foo_err);
 *     foo(1, &foo_err);
 *     if (HAS_ERROR(foo_err))
 *         CRASH(&foo_err); // or PRINT_ERROR(&foo_err)
 *     return EXIT_SUCCESS;
 * }
 *
 * int foo(int a, PICC_Error *error) {
 *     int f = 0;
 *     if (a < 0) {
 *         NEW_ERROR(error, ERR_WRONG_ARG);
 *     } else {
 *         ALLOC_ERROR(bar_err);
 *         int b = bar(-a, &bar_err);
 *         if (HAS_ERROR(bar_err))
 *             ADD_ERROR(error, bar_err, ERR_FOO_FAILED);
 *         else
 *             f = a + b;
 *     }
 *     return f;
 * }
 *
 * int bar(int b, PICC_Error *error) {
 *     if (b < 0)
 *         NEW_ERROR(error, ERR_WRONG_ARG);
 *     else
 *         b++;
 *     return b;
 * }
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <assert.h>
#include <errors.h>

extern static char *PICC_error_messages[PICC_NB_ERRORS + 1];

// An error stack.
typedef struct _PICC_Error {
    enum _PICC_ErrorId id;  // ID of the error (0 if the error didn't occured).
    char *file;      // File where the error occured.
    int line;        // Line in <file> where the error occured.
    struct _PICC_Error *prev; // Link to the previous error (NULL if this is the first).
} PICC_Error;

// macros used to handle the errors
#define ASSERT(test) \
    assert(test);

#define ALLOC_ERROR(error) \
    PICC_Error error = (PICC_Error){.id = 0, .file = NULL, .line = 0, .prev = NULL}

#define HAS_ERROR(error) \
    error.id > 0

#define NEW_ERROR(error, id) \
    PICC_init_error(error, id, __FILE__, __LINE__)

#define ADD_ERROR(error, prev, id) \
    PICC_add_error(error, prev, id, __FILE__, __LINE__)

#define FORWARD_ERROR(error, prev) \
    PICC_forward_error(error, prev)

#define CRASH(error) \
    PICC_crash(error, __FILE__, __FUNCTION__)

#define PRINT_ERROR(error) \
    PICC_print_error(error, __FILE__, __FUNCTION__)

// underlying error functions
extern void PICC_init_error(PICC_Error *error, PICC_ErrorId id, const char *file, int line);
extern void PICC_add_error(PICC_Error *error, PICC_Error prev_error, PICC_ErrorId id, const char *file, int line);
extern PICC_Error *PICC_copy_error(const PICC_Error error);
extern void PICC_crash(PICC_Error *error, const char *file, const char *fct);
extern void PICC_print_error(PICC_Error *error, const char *file, const char *fct);
extern void PICC_forward_error(PICC_Error *error, PICC_Error prev_error);
extern const char *PICC_get_error_message(PICC_ErrorId id);
extern void PICC_free_error(PICC_Error *error);

#endif
