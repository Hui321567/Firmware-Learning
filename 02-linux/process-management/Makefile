CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

TARGETS = fork_demo pipe_demo

all:$(TARGETS)

fork_demo: fork_demo.c
	$(CC) $(CFLAGS) -o $@ $<

pipe_demo: pipe_demo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

.PHONY: all clean 