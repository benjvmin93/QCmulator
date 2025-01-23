# QCmulator

This project aims to emulate a quantum computer as a state vector emulation using C language. It aims to be as efficient as possible in terms of memory management and computing speed.

It groups skills I have acquired during my studies at EPITA (C programming / quantum technologies) and my working experience at Inria.
Developing everything from scratch emphasizing efficiency in term of runtime and memory management made this project very enjoyable to implement.

# Architecture

```
  - src/*.c : source files
  - src/headers/*.h : contains all the header files
  - src/utils/*.c : contains utility functions
```

To represent complex matrices or vectors, I choosed using `double complex *` so it remains a very simple data type to treat as a 1D pointer.

# TODO

1. Finish simulator implementation: make sure everything works fine by adding a bunch of unit tests.
2. Add a circuit model overlay to run instruction with the statevector simulator.
3. Develop a benchmark to compare the efficiency of the simulator in order to assess bottlenecks and eventually start optimizing processes (multithreading, adhoc functions...)
4. Eventually build a Python API to make a gateway between the C project and a Python program.
