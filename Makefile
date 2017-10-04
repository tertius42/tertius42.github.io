CC=gcc
CCFLAGS=-Wall -g -fuse-ld=gold -std=c11

.c.o:
	$(CC) $(CCFLAGS) -c $<

wrapper:wrapper.o
	$(CC) $(CCFLAGS) $< -o$@

wrapper.o:wrapper.c

clean:
	rm -vf wrapper wrapper.o post*

