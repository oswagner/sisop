CC=gcc
CFFLAGS=-Wall

run: build
	./exe3

build: exe3.c
	${CC} ${CFLAGS1} -o exe3 exe3.c

clean:
	rm -rf exe3