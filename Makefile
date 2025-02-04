CC = gcc -std=c99
AR = ar
CFLAGS = -Wextra -Wall -pedantic -L/home/benjamin/miniconda3/envs/qcmulator/lib -I/home/benjamin/miniconda3/envs/qcmulator/include/python3.9
LIBS = -lm -lpython3.9
LIBTEST = -lcunit

# Source files
FILES = python/qcmulator.c src/*.c src/utils/*.c
UTIL_FILES = src/utils/alloc.c src/utils/list.c

# Test files
STATEVEC_TEST = ${UTIL_FILES} src/statevec.c src/gate.c src/projector.c tests/test_statevec.c
GATE_TEST = ${UTIL_FILES} src/gate.c tests/test_gate.c
LIST_TEST = ${UTIL_FILES} tests/test_list.c
CIRCUIT_TEST = ${UTIL_FILES} src/gate.c src/projector.c src/circuit.c tests/test_circuit.c

# Build targets
LIB_SHARED = libquantum.so

all: ${LIB_SHARED}

# Build the library
${LIB_SHARED}: ${FILES}
	${CC} ${CFLAGS} -fPIC -shared -o ${LIB_SHARED} ${FILES} ${LIBS}

# Tests
test-statevec: ${LIB_SHARED}
	${CC} ${CFLAGS} -fsanitize=address -o run-test-statevec -g ${STATEVEC_TEST} ${LIB_SHARED} ${LIBTEST} ${LIBS}

test-gate: ${LIB_SHARED}
	${CC} ${CFLAGS} -fsanitize=address -o run-test-gate -g ${GATE_TEST} ${LIB_SHARED} ${LIBTEST} ${LIBS}

test-list: ${LIB_SHARED}
	${CC} ${CFLAGS} -fsanitize=address -o run-test-list -g ${LIST_TEST} ${LIB_SHARED} ${LIBTEST} ${LIBS}

test-circuit: ${LIB_SHARED}
	${CC} ${CFLAGS} -fsanitize=address -o run-test-circuit -g ${CIRCUIT_TEST} ${LIB_SHARED} ${LIBTEST} ${LIBS}

# Debug build
debug: ${LIB_SHARED}
	${CC} ${CFLAGS} -fsanitize=address -o dbg -g main.c src/*.c ${LIB_SHARED} ${LIBS}

# Clean up
clean:
	rm -f main run-test-statevec run-test-gate run-test-list run-test-circuit dbg *.so ${LIB_SHARED}
