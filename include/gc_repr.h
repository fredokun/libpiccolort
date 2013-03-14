#ifndef GC_REPR_H
#define GC_REPR_H

#include <gc.h>
#include <scheduler_repr.h>

struct _PICC_Handle
{
    int global_rc;
    PICC_Lock *lock;
    PICC_Reclaimer reclaim;
};

bool PICC_GC2(PICC_SchedPool* sched);

#endif
