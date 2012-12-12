/**
 * @file errors.h
 * List of errors that can occur in the runtime.
 *
 * @author Mickaël MENU
 */


// Number of errors defined.
#define NB_ERRORS 100

// List of all errors defined.
enum PIT_ErrorId {
	ERR_KERNEL_ERROR,
	ERR_DEADLOCK,

	ERR_ALLOC_QUEUE_CELL,
	ERR_READY_QUEUE_PUSH,
	ERR_READY_QUEUE_ADD,
	ERR_WAIT_QUEUE_PUSH,
	ERR_WAIT_QUEUE_ADD,
	ERR_WAIT_QUEUE_PUSH_OLD,
	
	ERR_NULLPOINTER_PITHREAD,
	ERR_NULLPOINTER_COMMIT,
	ERR_NULLPOINTER_COMMITLISTELEM,
	ERR_NULLPOINTER_COMMITLIST,
	ERR_INVALID_COMMIT
};

// List of all error messages in order.
static const char *error_messages[NB_ERRORS + 1] = {
	"No error",

	"Released a free mutex",
	"Scheduler deadlock",

	"Can't alloc a new queue cell.",
	"Can't push the PiThread on the ready queue.",
	"Can't append the PiThread on the ready queue.",
	"Can't push the PiThread on the wait queue.",
	"Can't append the PiThread on the wait queue.",
	"Can't push the PiThread on the old wait queue."
};
