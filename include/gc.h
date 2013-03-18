#ifndef GC_H
#define GC_H

#include <error.h>
#include <scheduler_repr.h>
#include <concurrent.h>

typedef struct _PICC_Handle PICC_Handle;
//I known the convention doesn't include the pointer in the type
//but I think we have to make the function pointers the exception
typedef void (*PICC_Reclaimer) (PICC_Handle *handled, PICC_Error *error);
//see comments in gc_repr.h for more informations

extern void PICC_handle_incr_ref_count(PICC_Handle *h);
extern void PICC_handle_dec_ref_count(PICC_Handle **h);

extern bool PICC_GC2(PICC_SchedPool* sched);

#endif
