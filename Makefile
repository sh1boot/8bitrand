CC=gcc
CFLAGS := -std=c99 -pedantic -Wall
CFLAGS += -O2

TARGETS=8bitrand

all: $(TARGETS)

8bitrand: test.o 8bitrand.o tables.o

clean:
	rm -rf $(TARGETS) *.o
