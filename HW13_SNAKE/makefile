CC = gcc
CFLAGS = -Wall -std=c99 -lncurses -DNCURSES_STATIC

#gcc -Wall -static -o snake.exe snake.c -lncurses -DNCURSES_STATIC
PHONY: 		all
all:		snake.o
			$(CC) -o snake.exe $^ $(CFLAGS)

snake.o:	snake.c
			$(CC) $(CFLAGS) -c $^ -o $@
