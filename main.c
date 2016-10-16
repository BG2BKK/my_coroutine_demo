#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ucontext.h>

#include "coroutine.h"
#include "utils.h"

static ucontext_t uctx_main;

typedef enum {
	STATE_ENTRY = 0,
	STATE_S1,
	STATE_S2,
	STATE_DONE,
} FSM_STATE;

typedef enum {
	SIGNAL_A = 0,
	SIGNAL_B,
	SIGNAL_C,
} FSM_SIGNAL;

typedef struct fsm {
	char stack_[STACK_SIZE];
	ucontext_t uctx;
	cfunc func;
	FSM_STATE state;
} fsm_t;

void action_1(FSM_STATE state, FSM_SIGNAL signal) {
	printf("current state: %d, get signal: %d, ", state, signal);
	printf("switch to STATE_S1\n");
}

void action_2(FSM_STATE state, FSM_SIGNAL signal) {
	printf("current state: %d, get signal: %d, ", state, signal);
	printf("swithc to STATE_S2\n");
}

void action_3(FSM_STATE state, FSM_SIGNAL signal) {
	printf("current state: %d, get signal: %d, ", state, signal);
	printf("switch to STATE_DONE and stop FSM\n");
}

void action_0(FSM_STATE state, FSM_SIGNAL signal) {
	printf("current state: %d, get signal: %d, ", state, signal);
	printf("switch to STATE_ENTRY\n");
}

void action_state_stay(FSM_STATE state, FSM_SIGNAL signal) {
	printf("current state: %d, get signal: %d, ", state, signal);
	printf("FSM state not change\n");
}

void state_tran(fsm_t *fsm, FSM_SIGNAL signal) {

	FSM_STATE state = fsm->state;

	switch(state) {
		case STATE_ENTRY:
			switch(signal) {
				case SIGNAL_A:
					action_1(state, signal);
					fsm->state = STATE_S1;
					break;
				default:
					action_state_stay(state, signal);
					break;
			}
			swapcontext(&fsm->uctx, &uctx_main);
			break;
		case STATE_S1:
			switch(signal) {
				case SIGNAL_A:
					action_3(state, signal);
					fsm->state = STATE_DONE;
					break;
				case SIGNAL_B:
					action_2(state, signal);
					fsm->state = STATE_S2;
					break;
				default:
					action_state_stay(state, signal);
					break;
			}
			swapcontext(&fsm->uctx, &uctx_main);
			break;
		case STATE_S2:
			switch(signal) {
				case SIGNAL_A:
					action_0(state, signal);
					fsm->state = STATE_ENTRY;
					break;
				case SIGNAL_C:
					action_3(state, signal);
					fsm->state = STATE_DONE;
					break;
				default:
					action_state_stay(state, signal);
					break;
			}
			swapcontext(&fsm->uctx, &uctx_main );
			break;
		case STATE_DONE:
			printf("state done\n");
			break;
		default: 
			printf("invalid State, stop FSM\n");
			break;
	};
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
	printf("FSM Start\n");

	while(1) {
		printf("Please input your signal: ");
		int n = scanf("%d", &sig);
		if(n != 1) {
			handle_error("input signal error");
		}

		FSM_STATE state = resume(fsm, sig);
		if( state != STATE_DONE) {
			printf("current state: %d\n", state);
		} else {
			printf("current state: %d and FSM stop\n", state);
			break;
		}
	}
	delete_fsm(fsm);
}

