CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

DOCKER=docker run --rm -v ~/myrepo/9cc:/9cc -w /9cc --platform linux/amd64 compilerbook

9cc: $(OBJS)
	$(DOCKER) cc -o $@ $(OBJS) $(CFLAGS)

$(OBJS): 9cc.h
	$(DOCKER) cc -c -o $@ $(@:.o=.c) $(CFLAGS)

run: 9cc
	$(DOCKER) ./9cc $(ARG) > tmp.s

_test: 9cc
	./test.sh

test:
	$(DOCKER) make _test

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: run _test test clean