/**
 * @file entry.h
 * struct for entry.c
 *
 * @author Maxence WO
 * @author Dany Siriphol
 */

#ifndef ENTRY_H
#define ENTRY_H

#include <definitions.h>
#include <pi_thread.h>
#include <error.h>

struct PIT_Args
{
	PIT_Error error;
	PIT_SchedPool *sched_pool;
};

#endif
