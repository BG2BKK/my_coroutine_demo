#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ucontext.h>

#include "coroutine.h"
#include "utils.h"

static ucontext_t uctx_main, uctx_fsm;

char fsm_stack[STACK_SIZE];
char main_stack[STACK_SIZE];

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


void get_signal(int *sig) ;

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

void stop_fsm() {
	printf("FSM end\n");
}


void state_tran(FSM_SIGNAL signal) {

	static FSM_STATE state = STATE_ENTRY;

	switch(state) {
		case STATE_ENTRY:
			switch(signal) {
				case SIGNAL_A:
					action_1(state, signal);
					state = STATE_S1;

					FSM_SIGNAL sig;
					getcontext(&uctx_main);
					uctx_main.uc_stack.ss_sp = main_stack;
					uctx_main.uc_stack.ss_size = STACK_SIZE;
					uctx_main.uc_link = &uctx_fsm;
					makecontext(&uctx_main, (void (*)(void))get_signal, 1, &sig);

					swapcontext(&uctx_fsm, &uctx_main);
					break;
				default:
					break;
			}
			break;
		case STATE_S1:
			switch(signal) {
				case SIGNAL_A:
					action_3(state, signal);
					state = STATE_DONE;

					getcontext(&uctx_main);
					uctx_main.uc_stack.ss_sp = main_stack;
					uctx_main.uc_stack.ss_size = STACK_SIZE;
					uctx_main.uc_link = NULL;
					makecontext(&uctx_main, stop_fsm, 0);

					swapcontext(&uctx_fsm, &uctx_main);
					break;
				case SIGNAL_B:
					action_2(state, signal);
					state = STATE_S2;
					swapcontext(&uctx_fsm, &uctx_main );
					break;
				default:
					break;
			}
			break;
		case STATE_S2:
			switch(signal) {
				case SIGNAL_A:
					action_0(state, signal);
					state = STATE_ENTRY;
					swapcontext(&uctx_fsm, &uctx_main );
					break;
				case SIGNAL_C:
					action_3(state, signal);
					state = STATE_DONE;
					swapcontext(&uctx_fsm, &uctx_main );
					break;
				default:
					break;
			}
			break;
		case STATE_DONE:
			break;
		default: 
			printf("invalid State, stop FSM\n");
			break;
	};
}

void get_signal(int *sig) {
	printf("Please input your signal: ");
	int n = scanf("%d", sig);
	if(n != 1) {
		handle_error("input signal error");
	}

	getcontext(&uctx_fsm);
	uctx_fsm.uc_stack.ss_sp = fsm_stack;
	uctx_fsm.uc_stack.ss_size = STACK_SIZE;
	uctx_fsm.uc_link = &uctx_main;
	makecontext(&uctx_fsm, (void (*)(void))state_tran, 1, *sig);

	swapcontext(&uctx_main, &uctx_fsm);
}
int main()
{
	static int sig = 0;
	printf("FSM begin\n");

	getcontext(&uctx_main);
	uctx_main.uc_stack.ss_sp = main_stack;
	uctx_main.uc_stack.ss_size = STACK_SIZE;
	uctx_main.uc_link = &uctx_fsm;
	makecontext(&uctx_main, (void (*)(void))get_signal, 1, &sig);

	swapcontext(&uctx_fsm, &uctx_main);
}

