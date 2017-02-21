CC = gcc
CFLAGS = -c -static -nostdlib -fno-builtin -m32 -Os
OBJS = stdlib.o klib.o mem.o string.o

stdlib.o: stdlib.c
	$(CC) $(CFLAGS)  -o $@ $<	

klib.o: klib.c
	$(CC) $(CFLAGS)  -o $@ $<

mem.o: mem.c
	$(CC) $(CFLAGS)  -o $@ $<	

string.o: string.c
	$(CC) $(CFLAGS)  -o $@ $<

.PHONY: all
all:${OBJS}

.PHONY: clean
clean:
	rm -f *.o