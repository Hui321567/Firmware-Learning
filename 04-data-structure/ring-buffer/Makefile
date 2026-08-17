CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g -I.

TARGET = ring_buffer_test
SRCS = main.c ring_buffer.c

all:$(TARGET)

$(TARGET):$(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)

.PHONY: all clean
