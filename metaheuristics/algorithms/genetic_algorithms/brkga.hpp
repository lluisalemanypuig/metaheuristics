#pragma once

// C++ includes
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <queue>
using namespace std;

// Custom includes
#include "random/random_number_generator.hpp"
#include "algorithms/genetic_algorithms/chromosome.hpp"
#include "algorithms/genetic_algorithms/individual.hpp"
#include "algorithms/genetic_algorithms/rkga.hpp"
#include "algorithms/problem.hpp"
#include "misc/timing.hpp"

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random_gen;
using namespace mtime;

/* Uses an elite parent and a non-elite parent for the crossover operation.
 * Elite individuals are those that have the maximum fitness.
 * 
 */
class brkga : public genetic_algorithms {
	protected:
		double elite_copying_time;
		
		size_t N_ELITE;
		vector<pair<double, size_t> > elite_set;
		
		// Information display functions
		void print_elite_set() const;
		
		// Algorithm-related functions
		void get_two_parents(size_t& p1, size_t& p2);
		void copy_elite_individuals(const problem *p, population_set& next_gen, size_t& m);
		bool is_elite_individual(size_t idx) const;
		void track_elite_individuals();
		
		// Sanity check
		bool are_set_sizes_correct() const;
		
	public:
		brkga();
		brkga
		(
			size_t population_size,
			size_t mutant_population_size,
			size_t elite_set_size,
			size_t num_generations,
			size_t chrom_size,
			double inheritance_probability,
			random_number_generator *rng
		);
		virtual ~brkga();
		
		void reset_algorithm();
		
		const individual& get_best_individual() const;
		
		// pre: p is an empty instance of problem
		// post: p is a solution to the problem with a certain cost
		bool execute_algorithm(problem *p, double& c);
		
		void print_performance() const;
};

} // -- namespace algorithms
} // -- namespace metaheuristics

