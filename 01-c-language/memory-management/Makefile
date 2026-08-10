CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGETS = padding_test pointer_arithmetic dynamic_memory


all:$(TARGETS)

padding_test: padding_test.c
	$(CC) $(CFLAGS) -o $@ $<
pointer_arithmetic: pointer_arithmetic.c
	$(CC) $(CFLAGS) -o $@ $<
dynamic_memory: dynamic_memory.c
	$(CC) $(CFLAGS) -o $@ $<


clean:
	-rm -f $(TARGETS)

.PHONY: all clean