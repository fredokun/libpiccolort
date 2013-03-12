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
#include <value.h>
#include <commit_repr.h>
#include <pi_thread_repr.h>
#include <scheduler_repr.h>
#include <value_repr.h>

#define ASSERT_NO_ERROR() \
    ASSERT(!HAS_ERROR((*error)))

/*module Test/PingPong
def PingPong(i:chan<string>,o:chan<string>,msg:string) = i?(m), o!msg, PingPong(o,i,m)
def Main() = new(c1:chan<string>),new(c2:chan<string>),spawn{PingPong(c1,c2,"<PING>")}, c1!"<PONG>", PingPong(c2,c1,"<PONG>")*/

/*
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
*/

PICC_Value* eval_2(PICC_PiThread* pt){
PICC_copy_value( &  pt->val,  PICC_create_string_value( "<PONG>" ) );
   return pt->val;
}
PICC_Value* eval_1(PICC_PiThread* pt){
PICC_copy_value( &  pt->val,  pt->env[2] ); return pt->val;
}
void TestPingPong_PingPong(PICC_SchedPool* scheduler, 
PICC_PiThread* pt){
TestPingPong_PingPong_begin:
   switch(pt->pc){
   case 0:
      
      {
      PICC_TryResult tryresult;
      tryresult = PICC_TRY_DISABLED;
      int nbdisabled;
      nbdisabled = 0;
      PICC_KnownSet* chans;
      chans = PICC_create_empty_known_set(  );
      PICC_copy_value( &  pt->val,  PICC_create_bool_value( true ) );
      pt->enabled[0] = PICC_bool_of_bool_value( pt->val );
      if ( pt->enabled[0] ){
      {
         /* ------compile_try_in--------- */
         
         PICC_Channel* in_chan;
         in_chan = PICC_channel_of_channel_value( pt->env[0] );
         if ( PICC_known_set_add_channel( chans,  in_chan ) ){
         PICC_channel_value_acquire( pt->env[0] );
            }
         if ( PICC_channel_value_global_rc( pt->env[0] ) == 1 ){
         tryresult = PICC_TRY_DISABLED;
            goto end_of_try_4;
            }
         PICC_Commit* commit;
         PICC_CommitStatus ok;
         do{
         commit = PICC_fetch_output_commitment( in_chan );
         if ( commit == NULL ){
         tryresult = PICC_TRY_COMMIT;
            goto end_of_try_4;
            }
         do{
         ok = PICC_can_awake( commit->thread,  commit );
         if ( ok == PICC_CANNOT_ACQUIRE ){
         PICC_low_level_yield(  );
            }
         }while(ok == PICC_CANNOT_ACQUIRE);
         if ( ok == PICC_VALID_COMMIT ){
         PICC_Value* val;
            PICC_EvalFunction evalfunc;
            evalfunc = PICC_eval_func_of_output_commitment( commit );
            val = evalfunc( commit->thread );
            pt->env[3] = val;
            
            PICC_awake( scheduler,  commit->thread,  commit );
            tryresult = PICC_TRY_ENABLED;
            goto end_of_try_4;
            }
         }while(PICC_commit_list_is_empty( in_chan->outcommits ));
         }
         end_of_try_4:
         if ( tryresult == PICC_TRY_DISABLED ){
         pt->enabled[0] = false;
            nbdisabled = nbdisabled + 1;
            }else{
         if ( tryresult == PICC_TRY_ENABLED ){
            pt->fuel = pt->fuel - 1;
               if ( pt->fuel == 0 ){
               PICC_release_all_channels( chans );
                  /* ------compile_yield--------- */
                  
                  pt->pc = 1;
                  pt->fuel = PICC_FUEL_INIT;
                  PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
                  PICC_knowns_set_forget( pt->knowns ), it);
                   PICC_channel_dec_ref_count( &  chan );
                   PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
                    
                   END_KNOWNSET_FOREACH;
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
         /* ------compile_end--------- */
         
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_knows( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_forget( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         pt->status = PICC_STATUS_BLOCKED;
         return ;
         }
      if ( pt->enabled[0] ){
      {
         PICC_Channel* tmp_chan;
         tmp_chan = PICC_channel_of_channel_value( pt->env[0] );
         PICC_register_input_commitment( pt,  tmp_chan,  3,  1 );
         }
         }
      PICC_acquire( pt->lock );
      PICC_release_all_channels( chans );
      /* ------compile_wait--------- */
      
      pt->pc = PICC_INVALID_PC;
      pt->fuel = PICC_FUEL_INIT;
      PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
      PICC_knowns_set_forget( pt->knowns ), it);
       PICC_channel_dec_ref_count( &  chan );
       PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
        
       END_KNOWNSET_FOREACH;
      pt->status = PICC_STATUS_WAIT;
      PICC_wait_queue_push( scheduler->wait,  pt );
      PICC_release( pt->lock );
      return ;
      case 1:
      
      {
      PICC_TryResult tryresult;
      tryresult = PICC_TRY_DISABLED;
      int nbdisabled;
      nbdisabled = 0;
      PICC_KnownSet* chans;
      chans = PICC_create_empty_known_set(  );
      PICC_copy_value( &  pt->val,  PICC_create_bool_value( true ) );
      pt->enabled[0] = PICC_bool_of_bool_value( pt->val );
      if ( pt->enabled[0] ){
      {
         /* ------compile_try_out--------- */
         
         PICC_Channel* out_chan;
         out_chan = PICC_channel_of_channel_value( pt->env[1] );
         if ( PICC_known_set_add_channel( chans,  out_chan ) ){
         PICC_channel_value_acquire( pt->env[1] );
            }
         if ( PICC_channel_value_global_rc( pt->env[1] ) == 1 ){
         tryresult = PICC_TRY_DISABLED;
            goto end_of_try_3;
            }
         PICC_Commit* commit;
         PICC_CommitStatus ok;
         do{
         commit = PICC_fetch_input_commitment( out_chan );
         if ( commit == NULL ){
         tryresult = PICC_TRY_COMMIT;
            goto end_of_try_3;
            }
         do{
         ok = PICC_can_awake( commit->thread,  commit );
         if ( ok == PICC_CANNOT_ACQUIRE ){
         PICC_low_level_yield(  );
            }
         }while(ok == PICC_CANNOT_ACQUIRE);
         if ( ok == PICC_VALID_COMMIT ){
         PICC_copy_value( &  pt->val,  pt->env[2] );
            commit->thread->env[commit->content.in->refvar] = pt->val;
            PICC_awake( scheduler,  commit->thread,  commit );
            tryresult = PICC_TRY_ENABLED;
            goto end_of_try_3;
            }
         }while(PICC_commit_list_is_empty( out_chan->incommits ));
         }
         end_of_try_3:
         if ( tryresult == PICC_TRY_DISABLED ){
         pt->enabled[0] = false;
            nbdisabled = nbdisabled + 1;
            }else{
         if ( tryresult == PICC_TRY_ENABLED ){
            pt->fuel = pt->fuel - 1;
               if ( pt->fuel == 0 ){
               PICC_release_all_channels( chans );
                  /* ------compile_yield--------- */
                  
                  pt->pc = 2;
                  pt->fuel = PICC_FUEL_INIT;
                  PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
                  PICC_knowns_set_forget( pt->knowns ), it);
                   PICC_channel_dec_ref_count( &  chan );
                   PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
                    
                   END_KNOWNSET_FOREACH;
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
         /* ------compile_end--------- */
         
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_knows( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_forget( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         pt->status = PICC_STATUS_BLOCKED;
         return ;
         }
      if ( pt->enabled[0] ){
      {
         PICC_Channel* tmp_chan;
         tmp_chan = PICC_channel_of_channel_value( pt->env[1] );
         PICC_register_output_commitment( pt,  tmp_chan,  eval_1,  2 );
         }
         }
      PICC_acquire( pt->lock );
      PICC_release_all_channels( chans );
      /* ------compile_wait--------- */
      
      pt->pc = PICC_INVALID_PC;
      pt->fuel = PICC_FUEL_INIT;
      PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
      PICC_knowns_set_forget( pt->knowns ), it);
       PICC_channel_dec_ref_count( &  chan );
       PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
        
       END_KNOWNSET_FOREACH;
      pt->status = PICC_STATUS_WAIT;
      PICC_wait_queue_push( scheduler->wait,  pt );
      PICC_release( pt->lock );
      return ;
      case 2:
      
      {
      /* ------compile_call--------- */
      
      PICC_Value* args[3];
      PICC_knowns_set_forget_all( pt->knowns );
      PICC_copy_value( &  pt->val,  pt->env[1] );
      args[0] = pt->val;
      PICC_copy_value( &  pt->val,  pt->env[0] );
      args[1] = pt->val;
      PICC_copy_value( &  pt->val,  pt->env[3] );
      args[2] = pt->val;
      pt->env[0] = args[0];
      PICC_knowns_register( pt->knowns, 
                           PICC_channel_of_channel_value( args[0] ) );
      pt->env[1] = args[1];
      PICC_knowns_register( pt->knowns, 
                           PICC_channel_of_channel_value( args[1] ) );
      pt->env[2] = args[2];
      pt->proc = TestPingPong_PingPong;
      pt->pc = 0;
      pt->status = PICC_STATUS_CALL;
      return ;
      }
      }
      }
   }
}
void TestPingPong_Main(PICC_SchedPool* scheduler, 
PICC_PiThread* pt){
TestPingPong_Main_begin:
   switch(pt->pc){
   case 0:
      
      {
      PICC_TryResult tryresult;
      tryresult = PICC_TRY_DISABLED;
      int nbdisabled;
      nbdisabled = 0;
      PICC_KnownSet* chans;
      chans = PICC_create_empty_known_set(  );
      PICC_copy_value( &  pt->val,  PICC_create_bool_value( true ) );
      pt->enabled[0] = PICC_bool_of_bool_value( pt->val );
      if ( pt->enabled[0] ){
      {
         /* ------compile_try_new--------- */
         
         PICC_Channel* newchan;
         newchan = PICC_create_channel(  );
         pt->env[0] = PICC_create_channel_value( newchan );
         PICC_knowns_register( pt->knowns,  newchan );
         tryresult = PICC_TRY_ENABLED;
         }
         if ( tryresult == PICC_TRY_DISABLED ){
         pt->enabled[0] = false;
            nbdisabled = nbdisabled + 1;
            }else{
         if ( tryresult == PICC_TRY_ENABLED ){
            pt->fuel = pt->fuel - 1;
               if ( pt->fuel == 0 ){
               PICC_release_all_channels( chans );
                  /* ------compile_yield--------- */
                  
                  pt->pc = 1;
                  pt->fuel = PICC_FUEL_INIT;
                  PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
                  PICC_knowns_set_forget( pt->knowns ), it);
                   PICC_channel_dec_ref_count( &  chan );
                   PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
                    
                   END_KNOWNSET_FOREACH;
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
         /* ------compile_end--------- */
         
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_knows( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_forget( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         pt->status = PICC_STATUS_BLOCKED;
         return ;
         }
      
      PICC_acquire( pt->lock );
      PICC_release_all_channels( chans );
      /* ------compile_wait--------- */
      
      pt->pc = PICC_INVALID_PC;
      pt->fuel = PICC_FUEL_INIT;
      PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
      PICC_knowns_set_forget( pt->knowns ), it);
       PICC_channel_dec_ref_count( &  chan );
       PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
        
       END_KNOWNSET_FOREACH;
      pt->status = PICC_STATUS_WAIT;
      PICC_wait_queue_push( scheduler->wait,  pt );
      PICC_release( pt->lock );
      return ;
      case 1:
      
      {
      PICC_TryResult tryresult;
      tryresult = PICC_TRY_DISABLED;
      int nbdisabled;
      nbdisabled = 0;
      PICC_KnownSet* chans;
      chans = PICC_create_empty_known_set(  );
      PICC_copy_value( &  pt->val,  PICC_create_bool_value( true ) );
      pt->enabled[0] = PICC_bool_of_bool_value( pt->val );
      if ( pt->enabled[0] ){
      {
         /* ------compile_try_new--------- */
         
         PICC_Channel* newchan;
         newchan = PICC_create_channel(  );
         pt->env[1] = PICC_create_channel_value( newchan );
         PICC_knowns_register( pt->knowns,  newchan );
         tryresult = PICC_TRY_ENABLED;
         }
         if ( tryresult == PICC_TRY_DISABLED ){
         pt->enabled[0] = false;
            nbdisabled = nbdisabled + 1;
            }else{
         if ( tryresult == PICC_TRY_ENABLED ){
            pt->fuel = pt->fuel - 1;
               if ( pt->fuel == 0 ){
               PICC_release_all_channels( chans );
                  /* ------compile_yield--------- */
                  
                  pt->pc = 2;
                  pt->fuel = PICC_FUEL_INIT;
                  PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
                  PICC_knowns_set_forget( pt->knowns ), it);
                   PICC_channel_dec_ref_count( &  chan );
                   PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
                    
                   END_KNOWNSET_FOREACH;
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
         /* ------compile_end--------- */
         
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_knows( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_forget( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         pt->status = PICC_STATUS_BLOCKED;
         return ;
         }
      
      PICC_acquire( pt->lock );
      PICC_release_all_channels( chans );
      /* ------compile_wait--------- */
      
      pt->pc = PICC_INVALID_PC;
      pt->fuel = PICC_FUEL_INIT;
      PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
      PICC_knowns_set_forget( pt->knowns ), it);
       PICC_channel_dec_ref_count( &  chan );
       PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
        
       END_KNOWNSET_FOREACH;
      pt->status = PICC_STATUS_WAIT;
      PICC_wait_queue_push( scheduler->wait,  pt );
      PICC_release( pt->lock );
      return ;
      case 2:
      
      {
      PICC_TryResult tryresult;
      tryresult = PICC_TRY_DISABLED;
      int nbdisabled;
      nbdisabled = 0;
      PICC_KnownSet* chans;
      chans = PICC_create_empty_known_set(  );
      PICC_copy_value( &  pt->val,  PICC_create_bool_value( true ) );
      pt->enabled[0] = PICC_bool_of_bool_value( pt->val );
      if ( pt->enabled[0] ){
      {
         /* ------compile_try_spawn--------- */
         
         PICC_Value* args[3];
         PICC_PiThread* child;
         child = PICC_create_pithread( 4,  4,  4 );
         PICC_copy_value( &  pt->val,  pt->env[0] );
         args[0] = pt->val;
         pt->val = PICC_create_no_value();
         PICC_knowns_register( child->knowns, 
                              PICC_channel_of_channel_value( args[0] ) );
         PICC_channel_incr_ref_count( PICC_channel_of_channel_value( args[0] ) );
         child->env[0] = args[0];
         PICC_copy_value( &  pt->val,  pt->env[1] );
         args[1] = pt->val;
         pt->val = PICC_create_no_value();
         PICC_knowns_register( child->knowns, 
                              PICC_channel_of_channel_value( args[1] ) );
         PICC_channel_incr_ref_count( PICC_channel_of_channel_value( args[1] ) );
         child->env[1] = args[1];
         PICC_copy_value( &  pt->val,  PICC_create_string_value( "<PING>" ) );
         args[2] = pt->val;
         pt->val = PICC_create_no_value();
         
         child->env[2] = args[2];
         child->proc = TestPingPong_PingPong;
         child->pc = 0;
         child->status = PICC_STATUS_RUN;
         PICC_ready_queue_push( scheduler->ready,  child );
         tryresult = PICC_TRY_ENABLED;
         }
         if ( tryresult == PICC_TRY_DISABLED ){
         pt->enabled[0] = false;
            nbdisabled = nbdisabled + 1;
            }else{
         if ( tryresult == PICC_TRY_ENABLED ){
            pt->fuel = pt->fuel - 1;
               if ( pt->fuel == 0 ){
               PICC_release_all_channels( chans );
                  /* ------compile_yield--------- */
                  
                  pt->pc = 3;
                  pt->fuel = PICC_FUEL_INIT;
                  PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
                  PICC_knowns_set_forget( pt->knowns ), it);
                   PICC_channel_dec_ref_count( &  chan );
                   PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
                    
                   END_KNOWNSET_FOREACH;
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
         /* ------compile_end--------- */
         
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_knows( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_forget( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         pt->status = PICC_STATUS_BLOCKED;
         return ;
         }
      
      PICC_acquire( pt->lock );
      PICC_release_all_channels( chans );
      /* ------compile_wait--------- */
      
      pt->pc = PICC_INVALID_PC;
      pt->fuel = PICC_FUEL_INIT;
      PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
      PICC_knowns_set_forget( pt->knowns ), it);
       PICC_channel_dec_ref_count( &  chan );
       PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
        
       END_KNOWNSET_FOREACH;
      pt->status = PICC_STATUS_WAIT;
      PICC_wait_queue_push( scheduler->wait,  pt );
      PICC_release( pt->lock );
      return ;
      case 3:
      
      {
      PICC_TryResult tryresult;
      tryresult = PICC_TRY_DISABLED;
      int nbdisabled;
      nbdisabled = 0;
      PICC_KnownSet* chans;
      chans = PICC_create_empty_known_set(  );
      PICC_copy_value( &  pt->val,  PICC_create_bool_value( true ) );
      pt->enabled[0] = PICC_bool_of_bool_value( pt->val );
      if ( pt->enabled[0] ){
      {
         /* ------compile_try_out--------- */
         
         PICC_Channel* out_chan;
         out_chan = PICC_channel_of_channel_value( pt->env[0] );
         if ( PICC_known_set_add_channel( chans,  out_chan ) ){
         PICC_channel_value_acquire( pt->env[0] );
            }
         if ( PICC_channel_value_global_rc( pt->env[0] ) == 1 ){
         tryresult = PICC_TRY_DISABLED;
            goto end_of_try_5;
            }
         PICC_Commit* commit;
         PICC_CommitStatus ok;
         do{
         commit = PICC_fetch_input_commitment( out_chan );
         if ( commit == NULL ){
         tryresult = PICC_TRY_COMMIT;
            goto end_of_try_5;
            }
         do{
         ok = PICC_can_awake( commit->thread,  commit );
         if ( ok == PICC_CANNOT_ACQUIRE ){
         PICC_low_level_yield(  );
            }
         }while(ok == PICC_CANNOT_ACQUIRE);
         if ( ok == PICC_VALID_COMMIT ){
         PICC_copy_value( &  pt->val,  PICC_create_string_value( "<PONG>" ) );
            commit->thread->env[commit->content.in->refvar] = pt->val;
            PICC_awake( scheduler,  commit->thread,  commit );
            tryresult = PICC_TRY_ENABLED;
            goto end_of_try_5;
            }
         }while(PICC_commit_list_is_empty( out_chan->incommits ));
         }
         end_of_try_5:
         if ( tryresult == PICC_TRY_DISABLED ){
         pt->enabled[0] = false;
            nbdisabled = nbdisabled + 1;
            }else{
         if ( tryresult == PICC_TRY_ENABLED ){
            pt->fuel = pt->fuel - 1;
               if ( pt->fuel == 0 ){
               PICC_release_all_channels( chans );
                  /* ------compile_yield--------- */
                  
                  pt->pc = 4;
                  pt->fuel = PICC_FUEL_INIT;
                  PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
                  PICC_knowns_set_forget( pt->knowns ), it);
                   PICC_channel_dec_ref_count( &  chan );
                   PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
                    
                   END_KNOWNSET_FOREACH;
                  PICC_ready_queue_add( scheduler->ready,  pt );
                  return ;
                  }
               pt->pc = 4;
               goto TestPingPong_Main_begin;
               }
            }
         }else{
      nbdisabled = nbdisabled + 1;
         }
      if ( nbdisabled == 1 ){
      PICC_release_all_channels( chans );
         /* ------compile_end--------- */
         
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_knows( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
         PICC_knowns_set_forget( pt->knowns ), it);
          PICC_channel_dec_ref_count( &  chan );
           
          END_KNOWNSET_FOREACH;
         pt->status = PICC_STATUS_BLOCKED;
         return ;
         }
      if ( pt->enabled[0] ){
      {
         PICC_Channel* tmp_chan;
         tmp_chan = PICC_channel_of_channel_value( pt->env[0] );
         PICC_register_output_commitment( pt,  tmp_chan,  eval_2,  4 );
         }
         }
      PICC_acquire( pt->lock );
      PICC_release_all_channels( chans );
      /* ------compile_wait--------- */
      
      pt->pc = PICC_INVALID_PC;
      pt->fuel = PICC_FUEL_INIT;
      PICC_CHANNEL_KNOWNSET_FOREACH(chan, 
      PICC_knowns_set_forget( pt->knowns ), it);
       PICC_channel_dec_ref_count( &  chan );
       PICC_knowns_set_forget_to_unknown( pt->knowns,  chan );
        
       END_KNOWNSET_FOREACH;
      pt->status = PICC_STATUS_WAIT;
      PICC_wait_queue_push( scheduler->wait,  pt );
      PICC_release( pt->lock );
      return ;
      case 4:
      
      {
      /* ------compile_call--------- */
      
      PICC_Value* args[3];
      PICC_knowns_set_forget_all( pt->knowns );
      PICC_copy_value( &  pt->val,  pt->env[1] );
      args[0] = pt->val;
      PICC_copy_value( &  pt->val,  pt->env[0] );
      args[1] = pt->val;
      PICC_copy_value( &  pt->val,  PICC_create_string_value( "<PONG>" ) );
      args[2] = pt->val;
      pt->env[0] = args[0];
      PICC_knowns_register( pt->knowns, 
                           PICC_channel_of_channel_value( args[0] ) );
      pt->env[1] = args[1];
      PICC_knowns_register( pt->knowns, 
                           PICC_channel_of_channel_value( args[1] ) );
      pt->env[2] = args[2];
      pt->proc = TestPingPong_Main;
      pt->pc = 0;
      pt->status = PICC_STATUS_CALL;
      return ;
      }
      }
      }
      }
      }
   }
}

/**
 * Runs all runtime tests.
 */
void PICC_test_runtime()
{
    //PICC_main(4, TestPingPong_Main, 2, 2, 1);
    PICC_main(4, TestPingPong_Main, 4, 2, 1);
}
