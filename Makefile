CFLAGS=-std=c11 -g -static
SRCS=$(filter-out lib.c,$(wildcard *.c))
OBJS=$(SRCS:.c=.o)

9cc: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h

test: 9cc
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean
