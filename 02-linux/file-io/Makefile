CC = gcc
CFLAGES = -Wall -Wextra -std=c11 -g

TARGET = io_test

all:$(TARGET)

$(TARGET): io_test.c
	$(CC) $(CFLAGES) -o $@ $<

clean:
	rm -f $(TARGET)

.PHONY: all clean