
#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define STACK_SIZE 4096

typedef void (* cfunc)(void *);

typedef enum {
	COROUTINE_DEAD = 0,
	COROUTINE_READY,
	COROUTINE_RUNNING,
	COROUTINE_SUSPEND
}COROUTINE_STATUS;

struct coroutine {
	char stack_[STACK_SIZE];
	ucontext_t uctx;
	cfunc func;
	COROUTINE_STATUS status;
};

// struct coroutine * create_co(cfunc );
// int free_co(struct  coroutine *);

#endif
