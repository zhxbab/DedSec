CC = gcc
CFLAGS = -c -static -nostdlib -fno-builtin
OBJS = stdlib.o klib.o mem.o string.o

stdlib.o: stdlib.c
	$(CC) $(CFLAGS) -m32 -o $@ $<	

klib.o: klib.c
	$(CC) $(CFLAGS) -m32 -o $@ $<

mem.o: mem.c
	$(CC) $(CFLAGS) -m32 -o $@ $<	

string.o: string.c
	$(CC) $(CFLAGS) -m32 -o $@ $<

.PHONY: all
all:${OBJS}

.PHONY: clean
clean:
	rm -f *.o