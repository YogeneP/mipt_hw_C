CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -DNCURSES_STATIC -I/ucrt64/include
LDFLAGS = -static
LIBS = -lncurses

ifdef MESS
	CFLAGS += -DMESS
	TARGET_SUFFIX := _mess
endif

ifdef DEBUG
	CFLAGS += -DDEBUG
	TARGET_SUFFIX := $(TARGET_SUFFIX)_debug
endif

.PHONY: all clean

all: snake20$(TARGET_SUFFIX).exe

snake20$(TARGET_SUFFIX).exe: snake20$(TARGET_SUFFIX).o
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBS)

snake20$(TARGET_SUFFIX).o: snake20.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del *.exe *.o 2>nul || exit 0