# FM and LP based UTVPI Over Approximations

Implements algorithms to compute tight UTVPI overapproximations of polyhedra.

The algorithms provided are:
* Based on linear programming - LP0
* Based on Fourier-Motzkin elimination - FM1, FM2

Two implementations of LP0 have been provided, one using CPLEX and the other using PIPLib. 

Details of the algorithms can be found in the paper:

A. A. Patwardhan, P. Kumar, S. Rao, A. Miné and R. Upadrasta. On Tightest Octagonal Over-Approximations of Polyhedra.

## Setup
### Prerequisites
* CMake v3.12+
* Compiler for C and C++
* IBM CPLEX (for CPLEX version of LP0)
* PIPLib (for PIPLib version of LP0)

Install cplex at `$HOME/ibm/ILOG/CPLEX_Studio128`.

### Installation
To install FM1, FM2, and the CPLEX version of LP0, use:
```bash
$ mkdir build && cd build
$ cmake ..
$ make
```

To install the PIPLib version of LP0, use:
```bash
$ cd lp-pip
$ mkdir build && cd build
$ cmake ..
$ make
```
The path of the PIPLib installation can be passed to CMake using the `PIPLIB_PATH` variable.

## Data
The input polyhedron must be provided in the PolyLib/FMLib format.

## Running the program
The program accepts a polyhedron as input from the standard input and sends its UTVPI overapproximation as output to the standard output.

The executable (named `utvpi-oa` for FM1/FM2/CPLEX LP0 and `lp-pip` for PIPLib LP0) can be found after `make` in the `build` directory.
