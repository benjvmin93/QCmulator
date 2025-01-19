CC = gcc -std=c99
AR = ar
CFLAGS = -Wextra -Wall -pedantic
LIBS = -lm
LIBTEST = -lcunit

# Source files
FILES = main.c src/utils/*.c src/*.c
UTIL_FILES = src/utils/alloc.c src/utils/list.c src/utils/complex.c

# Test files
COMPLEX_TEST = ${UTIL_FILES} tests/test_complex.c
STATEVEC_TEST = ${UTIL_FILES} src/statevec.c src/gate.c tests/test_statevec.c
GATE_TEST = ${UTIL_FILES} src/gate.c tests/test_gate.c
LIST_TEST = ${UTIL_FILES} tests/test_list.c

# Build targets
LIB_NAME = libquantum.a

all: ${LIB_NAME} main

# Build the library
${LIB_NAME}: ${UTIL_FILES}
	${CC} ${CFLAGS} -c ${UTIL_FILES}
	${AR} rcs ${LIB_NAME} alloc.o list.o complex.o

# Build the main application using the library
main: ${LIB_NAME} main.c src/*.c
	${CC} ${CFLAGS} -o main main.c src/*.c ${LIB_NAME} ${LIBS}

# Tests
test-complex: ${LIB_NAME}
	${CC} -fsanitize=address -o run-test-complex -g tests/test_complex.c ${LIB_NAME} ${LIBTEST} ${LIBS}

test-statevec: ${LIB_NAME}
	${CC} -fsanitize=address -o run-test-statevec -g ${STATEVEC_TEST} ${LIB_NAME} ${LIBTEST} ${LIBS}

test-gate: ${LIB_NAME}
	${CC} -fsanitize=address -o run-test-gate -g ${GATE_TEST} ${LIB_NAME} ${LIBTEST} ${LIBS}

test-list: ${LIB_NAME}
	${CC} -fsanitize=address -o run-test-list -g ${LIST_TEST} ${LIB_NAME} ${LIBTEST} ${LIBS}

# Debug build
debug: ${LIB_NAME}
	${CC} ${CFLAGS} -fsanitize=address -o dbg -g main.c src/*.c ${LIB_NAME} ${LIBS}

# Clean up
clean:
	rm -f main run-test-complex run-test-statevec run-test-gate run-test-list dbg *.o ${LIB_NAME}
