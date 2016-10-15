
#include "coroutine.h"

struct coroutine *create_co( cfunc func) {
	struct coroutine *co = (struct coroutine *)malloc(sizeof(struct coroutine));
//	if (!co) 
//		handle_error("coroutine create error\n");
	co->stack_ = (char *)malloc(sizeof(char) * STACK_SIZE);
	co->uctx.uc_stack.ss_sp = co->stack_;
	co->uctx.uc_stack.ss_size = STACK_SIZE;
	co->uctx.uc_link = NULL;

	return co;
}

int free_co( struct coroutine *co) {
	if(co && co->stack_)
		free(co->stack_);
	free(co);
	return 1;
}

