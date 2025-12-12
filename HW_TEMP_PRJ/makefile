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

all : main.o temp_api.o
	$(CC) -o temp_log$(EXT) $^

main.o : main.c temp_api.h
	$(CC) $(CFLAGS) -c $< -o $@ 

temp_api.o : temp_api.c temp_api.h
	$(CC) $(CFLAGS) -c $< -o $@

clean : 
	$(RM) temp_log$(EXT) *.o