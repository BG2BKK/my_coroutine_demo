#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ucontext.h>

#include "coroutine.h"
#include "fsm.h"
#include "utils.h"

static ucontext_t uctx_main;

/*
 *
transmission task name: state_xx_signal_yy
when fsm stay at state xx, then receive signal yy, 
it will do action and switch to next state
 */
STATE_TRANS state_entry_signal_a= { trans_action, STATE_S1}; 
STATE_TRANS state_s1_signal_a	= { trans_action, STATE_DONE}; 
STATE_TRANS state_s1_signal_b	= { trans_action, STATE_S2}; 
STATE_TRANS state_s2_signal_a	= { trans_action, STATE_ENTRY}; 
STATE_TRANS state_s2_signal_c	= { trans_action, STATE_DONE}; 

pState_trans fsm_transtable[STATE_NUM][SIGNAL_NUM] = {

	{	// STATE_ENTRY
		&state_entry_signal_a,
		NULL,
		NULL,
	},

	{	// STATE_S1
		&state_s1_signal_a,
		&state_s1_signal_b,
		NULL,
	},

	{	// STATE_S2
		&state_s2_signal_a,
		NULL,
		&state_s2_signal_c,
	},

	{	// STATE_DONE
		NULL,
		NULL,
		NULL,
	},
};

void trans_action(fsm_t *fsm, FSM_SIGNAL signal, FSM_STATE next) {
	printf("current state: [%s], ", state_tostr(fsm->state));
	printf("get signal: <%s>, ", signal_tostr(signal));
	printf("switch to state [%s]\n", state_tostr(next));
}

void trans_state_stay(FSM_STATE state, FSM_SIGNAL signal) {
	printf("current state: [%s], get signal: <%d>, ", state_tostr(state), signal);
	printf("FSM state will not change\n");
}

void state_tran(fsm_t *fsm, FSM_SIGNAL signal) {
	if(signal >= 0 && signal < SIGNAL_NUM) {
		pState_trans trans = fsm_transtable[fsm->state][signal];
		if(trans == NULL) {
			trans_state_stay(fsm->state, signal);
		} else {
			(*(trans->action))(fsm, signal, trans->next);
			fsm->state = trans->next;
		}
	} else {
		trans_state_stay(fsm->state, signal);
	}
	yield(fsm);
//	swapcontext(&fsm->uctx, &uctx_main );
}

fsm_t *create_fsm() {
	fsm_t *fsm = (fsm_t *) malloc(sizeof(fsm_t));
	if(!fsm)
		return NULL;
	fsm->state = STATE_ENTRY;
	return fsm;
}

void delete_fsm(fsm_t *fsm) {
	free(fsm);
}

void yield(fsm_t *fsm) {
	swapcontext(&fsm->uctx, &uctx_main );
}

FSM_STATE resume(fsm_t *fsm, FSM_SIGNAL sig) {
	getcontext(&fsm->uctx);
	fsm->uctx.uc_stack.ss_sp = fsm->stack_;
	fsm->uctx.uc_stack.ss_size = STACK_SIZE;
	fsm->uctx.uc_link = &uctx_main;
	makecontext(&fsm->uctx, (void (*)(void))state_tran, 2, fsm, sig);

	swapcontext(&uctx_main, &fsm->uctx);
	return fsm->state;
}

int main()
{
	static int sig = 0;
	fsm_t *fsm = create_fsm();
	if(!fsm) {
		handle_error("create fsm error\n");
	}
	printf("FSM Start, current state: [%s]\n", state_tostr(fsm->state));

	while(1) {
		printf("Please input your signal [0-%d]: ", SIGNAL_NUM-1);
		int n = scanf("%d", &sig);
		if(n != 1) {
			handle_error("input signal error");
		}

		FSM_STATE state = resume(fsm, sig);

		printf("================\n");
		if( state != STATE_DONE) {
			printf("current state: %d\n", state);
		} else {
			printf("current state: %d and FSM stop\n", state);
			break;
		}
	}
	delete_fsm(fsm);
}

