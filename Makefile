# Cristian Castillo-Melara, G01436779
# CS 262, Lab Section 001
# Project 2

CC = gcc
CFLAGS = -Wall -pedantic-errors -std=c99
TARGET = p2_ccastil9_001

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

clean:
	rm -f $(TARGET)
