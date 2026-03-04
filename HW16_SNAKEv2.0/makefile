CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -lncurses -DNCURSES_STATIC

#gcc -Wall -static -o snake.exe snake.c -lncurses -DNCURSES_STATIC
.PHONY: 		all clean

all:		snake20.o
			$(CC) -o snake20.exe $^ $(CFLAGS)

snake20.o:	snake20.c
			$(CC) $(CFLAGS) -c $^ -o $@

clean:		
			del -Q *.exe *.o