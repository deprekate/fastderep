IDIR =./include
CC=gcc
CFLAGS=-g -Wall -O2 -Wno-unused-function -I$(IDIR)

SDIR=./src/

LIBS=-lm

all: fastderep

fastderep: $(SDIR)fastderep.c $(SDIR)bloom.c
	$(CC) $(CFLAGS) $^ -o $@ -lz -lm


