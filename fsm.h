#ifndef __FSM_H__
#define __FSM_H__


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

#define STATE_NUM 4
#define SIGNAL_NUM 3

typedef struct fsm {
	char stack_[STACK_SIZE];
	ucontext_t uctx;
	cfunc func;
	FSM_STATE state;
} fsm_t;

typedef void (* fsm_action)(fsm_t *fsm, FSM_SIGNAL signal, FSM_STATE next);

typedef struct {
	fsm_action action;
	FSM_STATE next;
} STATE_TRANS, *pState_trans;

void trans_action(fsm_t *fsm, FSM_SIGNAL signal, FSM_STATE next);
void trans_state_stay(FSM_STATE state, FSM_SIGNAL signal);

#endif
