# QCmulator

QCmulator is a project aimed at emulating a quantum computer using state vector simulation in the C programming language. The primary focus is on achieving high efficiency in terms of memory management and computing speed. This project leverages skills I acquired during my studies at EPITA in C programming and quantum technologies, as well as my working experience at Inria. Developing everything from scratch with an emphasis on runtime and memory efficiency made this project particularly enjoyable to implement.

## Architecture

The project is structured as follows:

```
- src/*.c : Source files containing the main implementation.
- src/headers/*.h : Header files defining the interfaces and data structures.
- src/utils/*.c : Utility functions to support the main implementation.
```

To represent complex matrices or vectors, the project uses `double complex *`, which allows for simple and efficient handling as a 1D pointer.

## Features

- **Efficient Memory Management**: Optimized for minimal memory usage.
- **High Computing Speed**: Designed to perform computations as quickly as possible.
- **Modular Design**: Clear separation of source files, headers, and utility functions for maintainability.

## TODO

1. **Complete Simulator Implementation**: Ensure the simulator works correctly by adding comprehensive unit tests.
2. **Circuit Model Overlay**: Implement a circuit model to run instructions with the state vector simulator.
3. **Benchmarking and Optimization**: Develop benchmarks to assess the simulator's efficiency, identify bottlenecks, and optimize processes (e.g., multithreading, ad-hoc functions).
4. **Python API**: Build a Python API to create a gateway between the C project and Python programs, enhancing usability and integration.

## Contributing

Contributions are welcome! If you have any ideas, suggestions, or bug reports, please open an issue or submit a pull request.

## License

This project is licensed under the GPL-3.0 License. See the [LICENSE](LICENSE) file for details.
