CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
TARGET = nand_sim

$(TARGET): main.c
	$(CC) $(CFLAGS) main.c -o $(TARGET)

.PHONY : clean

clean : 
	rm -f $(TARGET)