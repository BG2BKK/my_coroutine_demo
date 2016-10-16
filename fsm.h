#ifndef __FSM_H__
#define __FSM_H__


typedef enum {
	STATE_ENTRY = 0,
	STATE_S1,
	STATE_S2,
	STATE_DONE,

	STATE_DUMMY,
} FSM_STATE;

typedef enum {
	SIGNAL_A = 0,
	SIGNAL_B,
	SIGNAL_C,

	SIGNAL_DUMMY,
} FSM_SIGNAL;

static char *FSM_STATE_NAME[] = {
	"STATE_ENTRY", 
	"STATE_S1",
	"STATE_S2",
	"STATE_DONE",
	"STATE_DUMMY",
};

static char *FSM_SIG_NAME[] = {
	"SIGNAL_A",
	"SIGNAL_B",
	"SIGNAL_C",
	"SIGNAL_DUMMY",
};

#define STATE_NUM STATE_DUMMY
#define SIGNAL_NUM SIGNAL_DUMMY

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

void yield(fsm_t *fsm);
FSM_STATE resume(fsm_t *, FSM_SIGNAL);
char *signal_tostr(FSM_SIGNAL);
char *state_tostr(FSM_STATE);

char *signal_tostr(FSM_SIGNAL sig) {
	if(sig >= 0 && sig < SIGNAL_NUM) {
		return FSM_SIG_NAME[sig];
	} else {
		return FSM_SIG_NAME[SIGNAL_NUM - 1];
	}
}

char *state_tostr(FSM_STATE state) {
	if(state >= 0 && state < STATE_NUM) {
		return FSM_STATE_NAME[state];
	} else {
		return FSM_STATE_NAME[STATE_NUM - 1];
	}
}
#endif
