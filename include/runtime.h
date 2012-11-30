/*
##############################################################################
# runtime functions and compilation outlines
# Authors: Maxence WO
##############################################################################
*/

#ifndef DEF_RUNTIME
#define DEF_RUNTIME

/*################### LIST UTILS ####################*/

typedef struct
{
	union
	{
		Incommit in;
		Outcommit out;
	}
} Commit;

// PIT_commit_list_add : add the selected element at the end of the CommitList
void PIT_commit_list_add(Commit* clist, Commit c);

// PIT_commit_list_fetch : removes the first element from the commitlist and returns it
Commit PIT_commit_list_fetch(Commit* clist);

/*####################################################*/

/*################# QUEUE UTILS ######################*/

// PIT_ready_queue_push : push a PiThread on the top of a readyqueue
void PIT_ready_queue_push(Readyqueue rq, Pithread p);
// PIT_ready_queue_add : add a pithread a the end of a Readyqueue
void PIT_ready_queue_add(Readyqueue rq, Pithread p);
// PIT_readyqueue_pop : pop a pithread from the top of a readyqueue
Pithread PIT_readyqueue_pop(Readyqueue rq);
// PIT_readyqueue_size : returns the number of elements in a Readyqueue
int PIT_readyqueue_size(Readyqueue rq);

// PIT_waitqueue_push : pushes a pithread at the top of a Waitqueue
void PIT_waitqueue_push(Waitqueue wq, Pithread p);
// PIT_waitqueue_fetch : pop a selected Pithread from a waitqueue
Pithread PIT_waitqueue_fetch(Waitqueue wq, Pithread p);
// PIT_waitqueue_push_old : pushes a Pithread at the top of the old Pithreads in a Waitqueue
void PIT_waitqueue_push_old(Waitqueue wq, Pithread p);
// PIT_waitqueue_pop_old : pop the older Pithread from a waitqueue
Pithread PIT_waitqueue_pop_old(Waitqueue wq);
// PIT_waitqueue_size : returns the number of elements in a Waitqueue
int PIT_waitqueue_size(Waitqueue wq);
// PIT_waitqueue_max_active : returns the number of active elements in a Waitqueue
int PIT_waitqueue_max_active(Waitqueue wq);
// PIT_waitqueue_max_active_reset : reset the counter of active elemnts ina a waitqueue (ie: active elements are now old elements)
int PIT_waitqueue_max_active_reset(Waitqueue wq);

// PIT_knows_set_knows : returns a subset of all KNOWN-STATE in a knowsset
Knowsset PIT_knows_set_knows(Knowsset ks);
// PIT_knows_set_forget : returns a subset of all FORGET-STATE in a Knowsset
KnowsSet PIT_knows_set_forget(Knowsset ks);
// PIT_knows_set_forget_to_unknown : switches an element of a Knowsset from the FORGET-STATE to the UNKNOWN-STATE
// PIT_knows_register : looks for a channel in a Knowsset :
// - if the channel is in the Knowsset in KNOWN-STATE, it returns false
// - if the channel is in the Knowsset in FORGET-STATE, it switches it to KNOWN then  returns false
// - else it add the channel in the Knowsset (KNOWS-STATE) then returns true
int PIT_knows_register(Knowsset ks, Channel ch);

/*####################################################*/

/* Compilation outlines */

/* Constants */

// convert b:bool to pt.val
pt CompileExprBool(int b);

/* Variables */

// get the value at position v in pt.env and affect it to pt.val
// NB: pt.env[v] <=> pt.env[i] where i is the position of the variable v in pt.env (index solved in the compilation environnement)
pt CompileExprVar(int v);

/* unary operators */

pt CompileExprUOp()

#endif
