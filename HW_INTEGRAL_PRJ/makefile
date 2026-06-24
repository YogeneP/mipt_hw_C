ifeq ($(OS),Windows_NT)
	RM = del /Q
	EXT = .exe
else
	RM = rm -f
	EXT =
endif

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

.PHONY: all clean

all : main.c
	$(CC) -o int$(EXT) $^

clean : 
	$(RM) int$(EXT) *.o