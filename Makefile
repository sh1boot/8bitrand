CC=gcc
CFLAGS := -std=c99 -pedantic -Wall
CFLAGS += -O2

TARGETS=8bitrand period

all: $(TARGETS)

8bitrand: test.o 8bitrand.o tables.o examples.o

period: period.o tables.o

clean:
	rm -rf $(TARGETS) *.o
