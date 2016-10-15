
#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

#define STACK_SIZE 4096
typedef void* (* cfunc)(void *);

struct coroutine {
	ucontext_t uctx;
	char *stack_;
	cfunc func;
};

struct coroutine * create_co(cfunc );
int free_co(struct  coroutine *);

#endif
