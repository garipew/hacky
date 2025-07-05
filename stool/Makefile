ARCH?=32
CC=gcc
CFLAGS=-m$(ARCH) -g $(EXTRA)
STACKFLAGS=-fno-stack-protector -no-pie

bof: stool.c
	$(CC) $(CFLAGS) $(STACKFLAGS) -z execstack -o bof stool.c

ret2libc: stool.c
	$(CC) $(CFLAGS) $(STACKFLAGS) -o ret2libc stool.c

printable: stool.c
	$(CC) $(CFLAGS) $(STACKFLAGS) -z execstack -DPRINT -o printable stool.c

all: printable ret2libc bof

clean:
	rm -rf printable bof ret2libc

.PHONY: clean all bof ret2libc printable
