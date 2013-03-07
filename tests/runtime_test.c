/**
 * @file runtime_tests.c
 * Runtime unit tests
 *
 * This project is released under MIT License.
 *
 * @author Maxence WO
 * @author Joel HING
 */

#include <stdio.h>
#include <stdlib.h>
#include <runtime.h>
#include <queue.h>
#include <pi_thread_repr.h>
#include <scheduler_repr.h>
#include <value_repr.h>

#define ASSERT_NO_ERROR() \
    ASSERT(!HAS_ERROR((*error)))

/*Main env :[  ]
 module TestPingPong
def PingPong(i:chan<string>,o:chan<string>,msg:string) = i?(m:unknown),o!msg:unknown,:PingPong(i:unknown,o:unknown,msg:unknown)
def Main() = new(c1:chan<string>),new(c2:chan<string>),spawn{:PingPong(c1:unknown,c2:unknown,"<PING>")},0*/

PICC_Value* eval_1(PICC_PiThread* pt) {
    pt->val = pt->env[0]; return pt->val;
}

void TestPingPong_Main(PICC_SchedPool* scheduler, PICC_PiThread* pt){
TestPingPong_Main_begin:
    switch(pt->pc){
        case 0: {
            PICC_TryResult tryresult = PICC_TRY_ENABLED;
            printf("\tTestPingPong_Main action label 0\n");
            int nbdisabled;
            nbdisabled = 0;
            PICC_KnownSet *chans = PICC_create_empty_known_set();
            pt->val = (PICC_Value *)PICC_create_bool_value(true);
            pt->enabled[0] = pt->val;
            if ( pt->enabled[0] ){
                if ( tryresult == PICC_TRY_DISABLED ){
                    pt->enabled[0] = false;
                    nbdisabled = nbdisabled + 1;
                }else{
                    if ( tryresult == PICC_TRY_ENABLED ){
                        pt->fuel = pt->fuel - 1;
                        if ( pt->fuel == 0 ){
                            PICC_release_all_channels( chans );
                            pt->pc = 1;
                            pt->fuel = PICC_FUEL_INIT;
                            PICC_ready_queue_add( scheduler->ready,  pt );
                            return ;
                        }
                        pt->pc = 1;
                        goto TestPingPong_Main_begin;
                    }
                }
            }else{
                nbdisabled = nbdisabled + 1;
            }
            if ( nbdisabled == 1 ){
                PICC_release_all_channels( chans );
                pt->status = PICC_STATUS_BLOCKED;
                return ;
            }
            PICC_acquire( &(pt->lock) );
            PICC_release_all_channels( chans );
            pt->pc = PICC_INVALID_PC;
            pt->fuel = PICC_FUEL_INIT;
            pt->status = PICC_STATUS_WAIT;
            PICC_wait_queue_push( scheduler->wait,  pt );
            PICC_release( &(pt->lock) );
            return ;
        }
        case 1: {
            PICC_TryResult tryresult = PICC_TRY_ENABLED;
            printf("\tTestPingPong_Main action label 1\n");
            int nbdisabled;
            nbdisabled = 0;
            PICC_KnownSet *chans = PICC_create_empty_known_set();
            pt->val = (PICC_Value *)PICC_create_bool_value(true);
            pt->enabled[0] = pt->val;
            if ( pt->enabled[0] ){
                if ( tryresult == PICC_TRY_DISABLED ){
                    pt->enabled[0] = false;
                    nbdisabled = nbdisabled + 1;
                }else{
                    if ( tryresult == PICC_TRY_ENABLED ){
                        pt->fuel = pt->fuel - 1;
                        if ( pt->fuel == 0 ){
                            PICC_release_all_channels( chans );
                            pt->pc = 2;
                            pt->fuel = PICC_FUEL_INIT;
                            PICC_ready_queue_add( scheduler->ready,  pt );
                            return ;
                        }
                        pt->pc = 2;
                        goto TestPingPong_Main_begin;
                    }
                }
            }else{
                nbdisabled = nbdisabled + 1;
            }
            if ( nbdisabled == 1 ){
                PICC_release_all_channels( chans );
                pt->status = PICC_STATUS_BLOCKED;
                return ;
            }
            PICC_acquire( &(pt->lock) );
            PICC_release_all_channels( chans );
            pt->pc = PICC_INVALID_PC;
            pt->fuel = PICC_FUEL_INIT;
            pt->status = PICC_STATUS_WAIT;
            PICC_wait_queue_push( scheduler->wait,  pt );
            PICC_release( &(pt->lock) );
            return ;
        }
        case 2: {
            PICC_TryResult tryresult = PICC_TRY_ENABLED;
            printf("\tTestPingPong_Main action label 2\n");
            int nbdisabled;
            nbdisabled = 0;
            PICC_KnownSet *chans = PICC_create_empty_known_set();
            pt->val = (PICC_Value *)PICC_create_bool_value(true);
            pt->enabled[0] = pt->val;
            if ( pt->enabled[0] ){
                if ( tryresult == PICC_TRY_DISABLED ){
                    pt->enabled[0] = false;
                    nbdisabled = nbdisabled + 1;
                }else{
                    if ( tryresult == PICC_TRY_ENABLED ){
                        pt->fuel = pt->fuel - 1;
                        if ( pt->fuel == 0 ){
                            PICC_release_all_channels( chans );
                            pt->pc = 3;
                            pt->fuel = PICC_FUEL_INIT;
                            PICC_ready_queue_add( scheduler->ready,  pt );
                            return ;
                        }
                        pt->pc = 3;
                        goto TestPingPong_Main_begin;
                    }
                }
            }else{
                nbdisabled = nbdisabled + 1;
            }
            if ( nbdisabled == 1 ){
                PICC_release_all_channels( chans );
                pt->status = PICC_STATUS_BLOCKED;
                return ;
            }
            PICC_acquire( &(pt->lock) );
            PICC_release_all_channels( chans );
            pt->pc = PICC_INVALID_PC;
            pt->fuel = PICC_FUEL_INIT;
            pt->status = PICC_STATUS_WAIT;
            PICC_wait_queue_push( scheduler->wait,  pt );
            PICC_release( &(pt->lock) );
            return ;
        }
        case 3: {
            printf("\tTestPingPong_Main action label 3\n");
            pt->status = PICC_STATUS_ENDED;
            return ;
        }
    }
}

void TestPingPong_PingPong(PICC_SchedPool* scheduler, PICC_PiThread* pt){
TestPingPong_PingPong_begin:
    switch(pt->pc){
        case 0: {
            PICC_TryResult tryresult = PICC_TRY_ENABLED;
            printf("\tTestPingPong_PingPong action label 0\n");
            int nbdisabled;
            nbdisabled = 0;
            PICC_KnownSet *chans = PICC_create_empty_known_set();
            pt->val = (PICC_Value *)PICC_create_bool_value(true);
            pt->enabled[0] = pt->val;
            if ( pt->enabled[0] ){
                if ( tryresult == PICC_TRY_DISABLED ){
                    pt->enabled[0] = false;
                    nbdisabled = nbdisabled + 1;
                }else{
                    if ( tryresult == PICC_TRY_ENABLED ){
                        pt->fuel = pt->fuel - 1;
                        if ( pt->fuel == 0 ){
                            PICC_release_all_channels( chans );
                            pt->pc = 1;
                            pt->fuel = PICC_FUEL_INIT;
                            PICC_ready_queue_add( scheduler->ready,  pt );
                            return ;
                        }
                        pt->pc = 1;
                        goto TestPingPong_PingPong_begin;
                    }
                }
            }else{
                nbdisabled = nbdisabled + 1;
            }
            if ( nbdisabled == 1 ){
                PICC_release_all_channels( chans );
                pt->status = PICC_STATUS_BLOCKED;
                return ;
            }
            if ( pt->enabled[0] ){
                PICC_register_input_commitment( pt,  ((PICC_ChannelValue *)pt->env[0])->channel,  -1,  1 );
            }
            PICC_acquire( &(pt->lock) );
            PICC_release_all_channels( chans );
            pt->pc = PICC_INVALID_PC;
            pt->fuel = PICC_FUEL_INIT;
            pt->status = PICC_STATUS_WAIT;
            PICC_wait_queue_push( scheduler->wait,  pt );
            PICC_release( &(pt->lock) );
            return ;
        }
        case 1: {
            PICC_TryResult tryresult = PICC_TRY_ENABLED;
            printf("\tTestPingPong_PingPong action label 2\n");
            int nbdisabled;
            nbdisabled = 0;
            PICC_KnownSet *chans = PICC_create_empty_known_set();
            pt->val = (PICC_Value *)PICC_create_bool_value(true);
            pt->enabled[0] = pt->val;
            if ( pt->enabled[0] ){
                if ( tryresult == PICC_TRY_DISABLED ){
                    pt->enabled[0] = false;
                    nbdisabled = nbdisabled + 1;
                }else{
                    if ( tryresult == PICC_TRY_ENABLED ){
                        pt->fuel = pt->fuel - 1;
                        if ( pt->fuel == 0 ){
                            PICC_release_all_channels( chans );
                            pt->pc = 2;
                            pt->fuel = PICC_FUEL_INIT;
                            PICC_ready_queue_add( scheduler->ready,  pt );
                            return ;
                        }
                        pt->pc = 2;
                        goto TestPingPong_PingPong_begin;
                    }
                }
            }else{
                nbdisabled = nbdisabled + 1;
            }
            if ( nbdisabled == 1 ){
                PICC_release_all_channels( chans );
                pt->status = PICC_STATUS_BLOCKED;
                return ;
            }
            if ( pt->enabled[0] ){
                PICC_register_output_commitment( pt,  ((PICC_ChannelValue *)pt->env[0])->channel,  eval_1,  2 );
            }
            PICC_acquire( &(pt->lock) );
            PICC_release_all_channels( chans );
            pt->pc = PICC_INVALID_PC;
            pt->fuel = PICC_FUEL_INIT;
            pt->status = PICC_STATUS_WAIT;
            PICC_wait_queue_push( scheduler->wait,  pt );
            PICC_release( &(pt->lock) );
            return ;
        }
        case 2: {
            printf("\tTestPingPong_PingPong action label 2 : Call for PingPong\n");
            PICC_Value *args[3];
            PICC_knowns_set_forget_all( pt->knowns );
            pt->val = pt->env[0];
            args[0] = pt->val;
            pt->val = pt->env[1];
            args[1] = pt->val;
            pt->val = pt->env[2];
            args[2] = pt->val;
            pt->env[0] = args[0];
            pt->env[1] = args[1];
            pt->env[2] = args[2];
            pt->proc = TestPingPong_PingPong;
            pt->pc = 0;
            pt->status = PICC_STATUS_CALL;
            return ;
        }
    }
}


/**
 * Runs all runtime tests.
 */
void PICC_test_runtime()
{
    PICC_main(4, TestPingPong_Main, 2, 2, 1);
}
