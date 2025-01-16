CC = gcc

CFLAGS=-std=c99 -Wextra -Wall
FILES=main.c
TEST=src/utils/alloc.c src/utils/complex.c tests/test_complex.c 
LIBTEST=-lcunit
LIBS=-lm

all:
	${CC} ${CFLAGS} -pedantic -o main ${FILES} ${LIBS}
test:
	${CC} -fsanitize=address -o run-tests -g ${TEST} ${LIBTEST} ${LIBS}
debug:
	${CC} ${CFLAGS} -fsanitize=address -o dbg -g ${FILES} ${LIBS}
clean:
	rm -f main run-tests test dbg