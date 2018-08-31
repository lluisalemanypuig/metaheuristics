#pragma once

// C++ includes
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// C++ includes
#include <limits>
using namespace std;

// Custom includes
#include "random/random_number_generator.hpp"
#include "algorithms/genetic_algorithms/chromosome.hpp"
#include "algorithms/genetic_algorithms/individual.hpp"
#include "algorithms/metaheuristic.hpp"
#include "algorithms/problem.hpp"
#include "misc/timing.hpp"

namespace metaheuristics {
namespace algorithms {

using namespace mtime;
using namespace structures;
using namespace random_gen;

class genetic_algorithms : public metaheuristic {
	private:
	protected:
		size_t pop_size;	// population size
		size_t N_MUTANT;	// mutant population size
		size_t N_GEN;		// number of generations
		size_t chrom_size;	// chromosome size
		double in_prob;	// inheritance probability
		
		double total_time, initial_time, crossover_time, mutant_time;
		
		typedef vector<individual, allocator_individual<individual> > population_set;
		population_set population;
		
		// Information display functions
		void print_current_population() const;
		
		// Algorithm-related functions
		void initialize_population(const problem *p);
		void generate_mutants(const problem *p, size_t A, size_t B, population_set& next_gen, size_t& m);
		
		virtual void get_two_parents(size_t& p1, size_t& p2) = 0;
		void generate_crossovers(const problem *p, population_set& next_gen, size_t& m);
		
		void evaluate_individual(const problem *p, individual& i) const;
		void generate_mutant(const problem *p, individual& i);
		
		// son is the crossover of individuals par1_idx and par2_idx:
		//	if rand() < inheritance probability then
		// 		choose gene of parent 1 (par1_idx)
		//	else
		//		choose gene of parent 2 (par2_idx)
		void crossover(const problem *p, size_t par1_idx, size_t par2_idx, individual& son);
	
	public:
		genetic_algorithms();
		genetic_algorithms
		(
			size_t population_size,
			size_t mutant_population_size,
			size_t num_generations,
			size_t chrom_size,
			double inheritance_probability,
			random_number_generator *rng
		);
		virtual ~genetic_algorithms();
		
		// Sets the random number generator. The caller is made responsible
		// for freeing this object.
		void set_rng(random_number_generator *rng);
		
		virtual void reset_algorithm();
		
		// Returns the individual with the greatest fitness
		virtual const individual& get_best_individual() const;
		const individual& get_individual(size_t i) const;
		
		size_t population_size() const;
		
		// Executes a genetic algorithm. The solution stored in p maximises
		// the value of the function evaluate.
		// pre: p is an empty instance of the problem
		virtual bool execute_algorithm(problem *p, double& c) = 0;
		
		virtual void print_performance() const = 0;
};

}
}

