CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -pthread

TARGET = thread_demo

all:$(TARGET)

$(TARGET):thread_demo.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean