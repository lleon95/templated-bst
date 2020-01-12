# P1.3 Exam - Advanced programming

This repository contains the solution to the exam for the Advanced Programming
course of the MHPC. The project is mainly to implement a Binary Search Tree
(BST) in C++, and then, compare its performance agains the `std::map` function
from the Standard Library.

This project has the following files:

* [Library - bst.hpp](src/bst.hpp)
* [Profiler - time.hpp](src/time.hpp)
* [Main Source - main.cpp](main.cpp)
* Some profiling results found in `reports/`

Moreover, you can find the report in [REPORT.md](REPORT.md).

## Compilation instructions

In order to compile the code, run in a terminal:

```bash
make
```

To execute the code, run:

```bash
# Number of elements to inseert
N_ELEMENTS=100
# Number of iterations for non-copy/move-semantics
N_ITER=100 
# Number of iterations for copy/move-semantics
N_ITER_SEMANTICS=100
./main.x $N_ELEMENTS $N_ITER $N_ITER_SEMANTICS
```

If you want to enable the verbosity, uncomment `#define ENABLE_VERBOSE` in
[main.cpp](main.cpp), line 14.

To verify memory leakage:

```bash
make mem-check
```

To generate all the reports and benchmarks:

```bash
./generate_report.sh
```

### Development environment

The code was run on a machine with the following characteristics:

* gcc version 7.4.0 (Support for C++17 is required)
* Ubuntu 18.04: Linux 5.0.0-37-generic
* CPU 2.3GHz Core i3 2nd Generation - 8GB RAM

**Disclaimer**: The timing results might change if the code is run on a machine
with different characteristics, especially if the hardware bugs were mitigated
(meltdown and spectre).

------------------------

**Student**: Luis G. Leon Vega - 2020
**Version**: 0.1.1