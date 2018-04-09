CC=gcc
CFLAGS=-pthread -Wall 

run: build
	./exe4

build: exe4.c
	${CC} ${CFLAGS} -o exe4 exe4.c

clean:
	rm -rf exe4
