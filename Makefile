CC = g++
CFLAGS = -O3 -s -DNDEBUG

.PHONY: all
all: teasort

.PHONY: test
test: teasort
	./teasort

.PHONY: clean
clean:
	rm -f teasort

teasort: teasort.cc
	$(CC) $(CFLAGS) ${<} -o ${@}

# vim:ts=4
