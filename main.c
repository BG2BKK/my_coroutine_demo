#include <stdio.h>
#include <stdbool.h>
#include <ucontext.h>

#include "coroutine.h"


static ucontext_t uctx_main;

int main()
{
	printf("hello coroutine\n");
}

