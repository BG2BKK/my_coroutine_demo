CC = gcc
LD = gcc
CFLAGS = -Wall -c 
LDFLAGS =

SRCS = $(wildcard *.c)
OBJS = $(patsubst %c, %o, $(SRCS))
TARGET = main 

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o $(TARGET)
	rm -f *~
