/**
 * @file errors.h
 * List of errors that can occur in the runtime.
 *
 * This project is released under MIT License.
 *
 * @author Mickaël MENU
 */

#ifndef ERRORS_H
#define ERRORS_H

#ifndef TESTS

// APPLICATION ERROR MESSAGES /////////////////////////////////////////////////

    // Number of errors defined.
    #define PICC_NB_ERRORS 500

    // List of all errors defined.
    typedef enum _PICC_ErrorId {
        ERR_KERNEL_ERROR,
        ERR_DEADLOCK,

        ERR_ALLOC_QUEUE_CELL,
        ERR_READY_QUEUE_PUSH,
        ERR_READY_QUEUE_ADD,
        ERR_WAIT_QUEUE_PUSH,
        ERR_WAIT_QUEUE_ADD,
        ERR_WAIT_QUEUE_PUSH_OLD,

        ERR_NOT_IMPLEMENTED,
        ERR_OUT_OF_MEMORY,
        ERR_COMMIT_CREATE,
        ERR_REGISTER_OUT_COMMIT,
        ERR_REGISTER_IN_COMMIT,
        ERR_THREAD_CREATE,
        ERR_SCHED_POOL_CREATE,

        ERR_MUTEX_ALREADY_UNLOCKED,

        ERR_NULLPOINTER_PITHREAD,
        ERR_NULLPOINTER_COMMIT,
        ERR_NULLPOINTER_COMMITLISTELEM,
        ERR_NULLPOINTER_COMMITLIST,

        ERR_INVALID_COMMIT,
        ERR_INVALID_VALUE,

    } PICC_ErrorId;

#else

// TESTS ERROR MESSAGES ///////////////////////////////////////////////////////

    // Number of errors defined.
    #define PICC_NB_ERRORS 500

    // List of all errors defined.
    typedef enum _PICC_ErrorId {
        ERR_TEST1
    } PICC_ErrorId;

#endif

#endif
