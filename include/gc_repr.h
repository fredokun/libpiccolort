#ifndef GC_REPR_H
#define GC_REPR_H

#include <gc.h>
#include <scheduler_repr.h>
#include <concurrent.h>

/* *
 * Interface for the gc to be able to handle all managed value the same way
 * this is now implemented by the PICC_StringHandle and the PICC_Channel
 */
struct _PICC_Handle
{
    int global_rc;
    PICC_Lock *lock;
    PICC_Reclaimer reclaim; // pointer to the proper free function
};

#define LOCK_HANDLE(c) \
    PICC_acquire(((c)->lock));

#define RELEASE_HANDLE(c) \
    PICC_release(((c)->lock));



bool PICC_GC2(PICC_SchedPool* sched);

#endif
