CFLAGS=-std=c11 -g -static
DOCKER=docker run --rm -v ~/myrepo/9cc:/9cc -w /9cc compilerbook

9cc: 9cc.c
	$(DOCKER) cc -o $@ $< $(CFLAGS)

run: 9cc
	$(DOCKER) ./9cc $(ARG) > tmp.s

_test: 9cc
	./test.sh

test:
	$(DOCKER) make _test

clean:
	rm -f 9cc *.o *~ tmp*



.PHONY: _test test clean