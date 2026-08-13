CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -pthread

TARGET = thread_demo cond_demo

all:$(TARGET)

thread_demo:thread_demo.c
	$(CC) $(CFLAGS) -o $@ $<

cond_demo:cond_demo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean