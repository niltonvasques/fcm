CC=gcc
LIB=-lm
CFLAGS=-O2

all : gen fcm pcm

gen : gen.o
	${CC} ${CFLAGS} -o gen gen.o ${LIB}

gen.o : gen.c
	${CC} ${CFLAGS} -c gen.c

fcm : fcm.o
	${CC} ${CFLAGS} -o fcm fcm.o ${LIB}

fcm.o : fcm.c
	${CC} ${CFLAGS} -c fcm.c

pcm : pcm.o
	${CC} ${CFLAGS} -o pcm pcm.o ${LIB}

pcm.o : pcm.c
	${CC} ${CFLAGS} -c pcm.c

#shared : shared.o
#	${CC} ${CFLAGS} -o shared shared.o ${LIB}
#
#shared.o : shared.c
#	${CC} ${CFLAGS} -c shared.c

clean:
	rm -f fcm pcm gen 
	rm -f *.o
	rm -f *~
