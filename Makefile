CC=gcc
CFLAGS := -std=c99 -pedantic -Wall
CFLAGS += -O2

TARGETS=8bitrand period

all: $(TARGETS)

8bitrand: test.o 8bitrand.o tables.o examples.o

period: period.o tables.o

test.o: test.c examples.h 8bitrand.h tables.h

examples.o: examples.c examples.h

8bitrand.o: 8bitrand.c 8bitrand.h

clean:
	rm -rf $(TARGETS) *.o
