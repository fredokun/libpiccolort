/**
 * @file errors.h
 * List of errors that can occur in the runtime.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */


// Number of errors defined.
#define PICC_NB_ERRORS 500

// List of all errors defined.
enum PICC_ErrorId {
    ERR_KERNEL_ERROR,
    ERR_DEADLOCK,

    ERR_ALLOC_QUEUE_CELL,
    ERR_READY_QUEUE_PUSH,
    ERR_READY_QUEUE_ADD,
    ERR_WAIT_QUEUE_PUSH,
    ERR_WAIT_QUEUE_ADD,
    ERR_WAIT_QUEUE_PUSH_OLD,

    ERR_OUT_OF_MEMORY,
    ERR_THREAD_CREATE,

    ERR_NULLPOINTER_PITHREAD,
    ERR_NULLPOINTER_COMMIT,
    ERR_NULLPOINTER_COMMITLISTELEM,
    ERR_NULLPOINTER_COMMITLIST,
    ERR_INVALID_COMMIT,
    ERR_INVALID_VALUE
};

// List of all error messages in order.
static const char *PICC_error_messages[PICC_NB_ERRORS + 1] = {
    "No error",

    "Released a free mutex",
    "Scheduler deadlock",

    "Can't alloc a new queue cell.",
    "Can't push the PiThread on the ready queue.",
    "Can't append the PiThread on the ready queue.",
    "Can't push the PiThread on the wait queue.",
    "Can't append the PiThread on the wait queue.",
    "Can't push the PiThread on the old wait queue.",

    "Out of memory.",
    "Can't create the thread."
};
