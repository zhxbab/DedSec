CC = gcc
CFLAGS = -c

test.o: test.c
	$(CC) $(CFLAGS) -m32 -o $@ $<	

.PHONY: all
all:test.o

.PHONY: clean
clean:
	rm -f *.o