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
 * int foo(int a, PIT_Error *error) {
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
 * int bar(int b, PIT_Error *error) {
 *     if (b < 0)
 *         NEW_ERROR(error, ERR_WRONG_ARG);
 *     else
 *         b++;
 *     return b;
 * }
 *
 * @author Mickaël MENU
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdlib.h>
#include <assert.h>
#include <errors.h>
#include <definitions.h>

// An error stack.
struct PIT_Error {
	PIT_ErrorId id;  // ID of the error (0 if the error didn't occured).
	char *file;      // File where the error occured.
	int line;        // Line in <file> where the error occured.
	PIT_Error *prev; // Link to the previous error (NULL if this is the first).
};

// macros used to handle the errors
#define ASSERT(test) \
	assert(test);

#define ALLOC_ERROR(error) \
	PIT_Error error = (PIT_Error){.id = 0, .file = NULL, .line = 0, .prev = NULL}

#define HAS_ERROR(error) \
	error.id > 0

#define NEW_ERROR(error, id) \
	PIT_init_error(error, id, __FILE__, __LINE__)

#define ADD_ERROR(error, prev, id) \
	PIT_add_error(error, prev, id, __FILE__, __LINE__)

#define FORWARD_ERROR(error, prev) \
	PIT_forward_error(error, prev)

#define CRASH(error) \
	PIT_crash(error, __FILE__, __FUNCTION__)

#define PRINT_ERROR(error) \
	PIT_print_error(error, __FILE__, __FUNCTION__)

// underlying error functions
extern void PIT_init_error(PIT_Error *error, PIT_ErrorId id, const char *file, int line);
extern void PIT_add_error(PIT_Error *error, PIT_Error prev_error, PIT_ErrorId id, const char *file, int line);
extern PIT_Error *PIT_copy_error(const PIT_Error error);
extern void PIT_crash(PIT_Error *error, const char *file, const char *fct);
extern void PIT_print_error(PIT_Error *error, const char *file, const char *fct);
extern void PIT_forward_error(PIT_Error *error, PIT_Error prev_error);
extern const char *PIT_get_error_message(PIT_ErrorId id);
extern void PIT_free_error(PIT_Error *error);

#endif
