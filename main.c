#include <stdio.h>
#include <stdbool.h>
#include <ucontext.h>

#include "coroutine.h"

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct msgbuf {
	char buf[STACK_SIZE * 4];
	int len;
	int pos;
	int last;
};

static ucontext_t uctx_main;

struct coroutine *producer, *consumer;
struct msgbuf *buffer;

void produce() {
	while(1) {
	}
}

void consume() {

}

int main()
{
//	producer = create_co(produce, uctx_main);
//	consumer = create_co(consume, uctx_main);
//
//	if( !consumer || !producer)
//		handle_error("create producer or consumer error");

	producer = (struct coroutine *)malloc(sizeof(struct coroutine));
	if( !producer)
		handle_error("create producer error");
	consumer = (struct coroutine *)malloc(sizeof(struct coroutine));
	if( !consumer)
		handle_error("create consumer error");

	buffer = (struct msgbuf *)malloc(sizeof(struct msgbuf));
	if( !buffer)
		handle_error("create msgbuf error");


	getcontext(&producer->uctx);
	producer->uctx.uc_stack.ss_sp = producer->stack_;
	producer->uctx.uc_stack.ss_size = STACK_SIZE;
//	producer->uctx.uc_link = &consumer->uctx;
	producer->uctx.uc_link = &uctx_main;

	getcontext(&consumer->uctx);
	consumer->uctx.uc_stack.ss_sp = consumer->stack_;
	consumer->uctx.uc_stack.ss_size = STACK_SIZE;
//	consumer->uctx.uc_link = &producer->uctx;
	consumer->uctx.uc_link = &uctx_main;

	makecontext(&(producer->uctx), produce, 0);
	makecontext(&(consumer->uctx), consume, 0);

	swapcontext(&uctx_main, &(producer->uctx));
}

