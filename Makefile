CC = gcc -std=c99

CFLAGS = -Wextra -Wall
FILES = main.c
COMPLEX_TEST = src/utils/alloc.c src/utils/complex.c tests/test_complex.c
STATEVEC_TEST = src/utils/alloc.c src/utils/complex.c src/statevec.c tests/test_statevec.c
GATE_TEST = src/utils/alloc.c src/utils/complex.c src/gate.c tests/test_gate.c
LIBTEST = -lcunit
LIBS = -lm

all:
	${CC} ${CFLAGS} -pedantic -o main ${FILES} ${LIBS}

test-complex:
	${CC} -fsanitize=address -o run-complex-tests -g ${COMPLEX_TEST} ${LIBTEST} ${LIBS}

test-statevec:
	${CC} -fsanitize=address -o run-statevec-tests -g ${STATEVEC_TEST} ${LIBTEST} ${LIBS}

test-gate:
	${CC} -fsanitize=address -o run-gate-tests -g ${GATE_TEST} ${LIBTEST} ${LIBS}

debug:
	${CC} ${CFLAGS} -fsanitize=address -o dbg -g ${FILES} ${LIBS}

clean:
	rm -f main run-complex-tests run-statevec-tests run-gate-tests dbg
