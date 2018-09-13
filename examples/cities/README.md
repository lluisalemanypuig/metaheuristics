# Locations, Cities and Centres

This example is part of the course MIRI-AMMM that the authors attended during
the second semester of the academic year 2016-2017 at the UPC.o

The code has been adapted to compile with the new changes to the metaheuristics
library, which can be found [here](https://github.com/lluisalemanypuig/metaheuristics).

There are several input files that can be used to test this example [here](https://github.com/lluisalemanypuig/metaheuristics/tree/master/examples/citites/inputs).

## Compilation

In order to compile and execute this example, first compile the metaheuristics library
following the instructions in [this Readme](https://github.com/lluisalemanypuig/metaheuristics/blob/master/README.md).
Then, issue the following commands:

		cd /path/to/lib/metaheuristics/examples/cities/build
		make -f Makefile release

This will create a directory in which one will find the executable file:

		cd /path/to/lib/metaheuristics/examples/cities/bin-release

## Execution

Once in the directory with the executable file, issue the following command
to see its usage:

		./cities --help

This is the result:

		Locations, Cities, and Centres
		Usage:
			[-h, --help]:        shows this help
			[-s, --seed]:        seed the random number generators
			[-i, --input] f:     the input file with the description of the instance
			[--optimal] o:       value of the optimal solution (extracted from the ILP solution
			[-a, --algorithm] a: the algorithm to be executed. Possible values:
				- local-search:  constructs an initial solution deterministically
								 (i.e., not randomly) and, following a given policy, improves
								 the solution exploring the neighbourhood
				- grasp:         aplies the GRASP metaheuristic, using the parameter alpha
								 for the randomised choice of candidates
				- rkga:          aplies the RKGA metaheuristic
				- brkga:         aplies the BRKGA metaheuristic

		Optional parametres:
		* For the algorithms:
			-> Local search algorithm:
				[--iter-local] i:      maximum number of iterations for the local search algorithm. Default: 10
				[--policy] p:          policy that the algorithm will apply. Default: Best
					Possible values:
					- First:           First improvement
					- Best:            Best improvement
			-> GRASP:
				[--alpha] a:           randomness parameter. Default = 1
				[--iter-local] i:      maximum number of iterations for the local search algorithm. Default: 10
				[--iter-grasp] i:      maximum number of iterations for the GRASP metaheuristic. Default: 10
				[--policy] p:          policy that the algorithm will apply. Default: Best
					Possible values:
					- First:           First improvement
					- Best:            Best improvement
			-> RKGA:
				[--num-gen] i:         maximum number of generations. Default: 10
				[--pop-size] s:        size of the total population. Default: 0
				[--mut-size] s:        size of the mutant population. Default: 0
				[--inher-prob] p:      probability of inheritance. Default: 0.5
			-> BRKGA:
				[--num-gen] i:         maximum number of generations. Default: 10
				[--pop-size] s:        size of the total population. Default: 0
				[--mut-size] s:        size of the mutant population. Default: 0
				[--elite-size] s:      size of the elite population set. Default: 0
				[--inher-prob] p:      probability of inheritance. Default: 0.5

One can see that there are options to use several heuristic algorithms and metaheuristics.
