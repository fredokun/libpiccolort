/**
 * @file errors.h
 * List of errors that can occur in the runtime.
 *
 * @author Mickaël MENU
 */

// Number of errors defined.
#define NB_ERRORS 1

// List of all errors defined.
enum PIT_ErrorId {
	ERR_ALLOC_QUEUE_CELL,
	ERR_READY_QUEUE_PUSH,
	ERR_READY_QUEUE_ADD
};

// List of all error messages in order.
static const char *error_messages[NB_ERRORS + 1] = {
	"No error",
	"Can't alloc a new queue cell.",
	"Can't push the PiThread on the ready queue."
	"Can't append the PiThread on the ready queue."
};
