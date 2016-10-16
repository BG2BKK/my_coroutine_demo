
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#define BUFFER_SIZE 4096
#define handle_error(msg) \
	do { printf("%s\n",msg); exit(1); } while (0)


typedef struct msgbuf {
	char buf[BUFFER_SIZE];
	int len;
	int pos;
	int last;
} msgbuf;

#endif
