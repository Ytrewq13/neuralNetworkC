CC=gcc
CFLAGS=-I. -lm
DEPS = $(wildcard *.h)
OBJ := $(patsubst %.c,%.o,$(wildcard *.c))

%.o : %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

a.out: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	rm $(OBJ)
