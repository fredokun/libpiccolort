/**
 * @file entry.h
 * struct for entry.c
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Dany Siriphol
 */

#ifndef ENTRY_H
#define ENTRY_H

#include <definitions.h>
#include <pi_thread.h>
#include <error.h>

struct PICC_Args
{
	PICC_Error error;
	PICC_SchedPool *sched_pool;
};

#endif
