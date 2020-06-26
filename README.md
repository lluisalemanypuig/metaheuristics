# metaheuristics library

This repository offers a small library with a collection of heuristics and metaheuristics
algorithms:
- Local Search
- GRASP: Greedy Randomsied Adaptative Search Procedure
- Genetic algorithms:
  - RKGA: Random-Key Genetic Algorithm
  - BRKGA: Biased Random-Key Genetic Algorithm

These procedures work with an interface named _problem_. The implementation of a few
methods of this class allows a flexible use of these metaheuristics.

## Using the library

In this repository one will find [examples](https://github.com/lluisalemanypuig/metaheuristics/tree/master/examples)
on how to use this library.

First, one has to download the library. Assume that it is downloaded into

    /path/to/lib/metaheurisitcs

## Documentation

For further details on how the library is implemented, the meaning of every
parameter, what methods should be implemented of the _problem_ interface for
every algorithm, generate the html documentation with Doxygen issuing the following
command:

    cd /path/to/lib/metaheurisitcs/
    doxygen doxyconfig/doxyfile

## Dependencies

### Tools

Since the Makefiles are already provided, a 'make' tool is needed to compile
the library and the examples.

Finally, a compiler that supports C++11 is mandatory.

## Compilation

In order to compile the library one needs a compiler that supports the C++11 standard, in particular
the flag -std=c++11, and the _make_ tool:

    cd /path/to/lib/metaheurisitcs/build
    make -f Makefile release

These commands will create a _metaheuristics-release_ directroy in the same directory as the project,
that is, in:

    /path/to/lib/metaheurisitcs/metaheuristics-release

After compiling, in this directory one will find a file called _libmetaheuristics.a_ intended for
static linkage with any software that needs to use this library. Include the appropriate flags in
your compilation rules to allow this linkage. Also, the user will need to include the -I flag with
the path to the root of the project, e.g.:

    -I/path/to/lib/metaheurisitcs

to make sure that all included files will be found by the compiler.
