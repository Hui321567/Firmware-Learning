CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = uart_demo
SRCS = main.c uart.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean