CC=gcc
LIB=-lm
CFLAGS=-O2 -I .
OBJ = gen.o fcm.o pcm.o main.o
DEPS= gnuplot.h shared.h fcm.h pcm.h

all : gen  main

gen : gen.o
	${CC} ${CFLAGS} -o gen gen.o ${LIB}

gen.o : gen.c
	${CC} ${CFLAGS} -c gen.c

fcm.o : fcm.c 
	${CC} ${CFLAGS} -c fcm.c

pcm.o : pcm.c
	${CC} ${CFLAGS} -c pcm.c

main : main.o pcm.o fcm.o
	gcc -o $@ $^ $(CFLAGS) $(LIB)

main.o : main.c ${DEPS}
	${CC} -c -o $@ $< ${CFLAGS}

clean:
	rm -f fcm pcm gen main
	rm -f *.o
	rm -f *~
