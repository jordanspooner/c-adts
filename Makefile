CC     = gcc
CFLAGS = -Wall -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -std=c99 -Werror -pedantic -O3

.PHONY = clean

linked_list: linked_list.o

clean:
	rm -f $(wildcard *.o)
	rm -f linked_list
