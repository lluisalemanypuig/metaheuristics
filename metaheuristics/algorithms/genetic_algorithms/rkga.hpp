#pragma once

// C++ includes
#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>
using namespace std;

// C++ includes
#include <limits>
using namespace std;

// Custom includes
#include "random/random_number_generator.hpp"
#include "algorithms/genetic_algorithms/genetic_algorithms.hpp"
#include "algorithms/genetic_algorithms/chromosome.hpp"
#include "algorithms/genetic_algorithms/individual.hpp"
#include "algorithms/problem.hpp"
#include "misc/timing.hpp"

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random_gen;
using namespace mtime;

class rkga : public genetic_algorithms {
	private:
		void get_two_parents(size_t& p1, size_t& p2);
		
	protected:
		
		// Sanity check
		bool are_set_sizes_correct() const;
		
	public:
		rkga();
		rkga
		(
			size_t population_size,
			size_t mutant_population_size,
			size_t num_generations,
			size_t chrom_size,
			double inheritance_probability,
			random_number_generator *rng
		);
		virtual ~rkga();
		
		// pre: p is an empty instance of problem
		// post: p is a solution to the problem with a certain cost
		bool execute_algorithm(problem *p, double& c);
		
		void print_performance() const;
};

}
}

