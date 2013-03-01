/**
 * @file runtime.h
 * File that contains headers of the runtime functions specified in the specification's document.
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 */

#ifndef RUNTIME_H
#define RUNTIME_H

#include <scheduler.h>
#include <pi_thread.h>
#include <error.h>

extern int PICC_GC2(PICC_SchedPool *sp, PICC_Error *error);
extern void PICC_main(int nb_core_threads, PICC_PiThreadProc *entrypoint);

#endif
