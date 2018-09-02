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
#include <metaheuristics/random/random_number_generator.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/chromosome.hpp>
#include <metaheuristics/algorithms/genetic_algorithms/individual.hpp>
#include <metaheuristics/algorithms/problem.hpp>
#include <metaheuristics/misc/time.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random;
using namespace mtime;

/**
 * @brief Implementation of the RKGA.
 * 
 * RKGA stands for Random-Key Genetic Algorithm.
 */
class rkga : public genetic_algorithm {
	private:
		/**
		 * @brief Implementation of the parents choice.
		 * 
		 * Choose two individuals at random from @ref population.
		 */
		void get_two_parents(size_t& p1, size_t& p2);
		
	protected:
		
		// Sanity check
		/**
		 * @brief Makes sure that the different populations sizes are correct.
		 * 
		 * Checks that @ref N_MUTANT < @ref pop_size. Basically, a sanity check.
		 */
		bool are_set_sizes_correct() const;
		
	public:
		/// Default constructor
		rkga();
		/**
		 * @brief Constructor with parameters.
		 * @param p_size The total size of the population (see @ref pop_size).
		 * @param m_size The amount of mutant individuals (see @ref MUTANT_SIZE).
		 * @param n_gen The number of generations (see @ref N_GEN).
		 * @param chrom_size The size of each individual's chromosome (see @ref chrom_size).
		 * @param i_prob Inheritance probability (see @ref in_prob).
		 * @param rand_gen The random number generator (see @ref rng).
		 */
		rkga
		(
			size_t p_size,
			size_t m_size,
			size_t n_gen,
			size_t chrom_size,
			double i_prob,
			random_number_generator *rand_gen
		);
		/// Destructor.
		virtual ~rkga();
		
		// pre: p is an empty instance of problem
		// post: p is a solution to the problem with a certain cost
		/**
		 * @brief Executes the RKGA algorithms
		 * 
		 * The RKGA executes the following steps:
		 * - Initialise the population with @ref pop_size mutants
		 * - For as many generations as @ref N_GEN:
		 * 		- Generate mutants in the range [0, @ref N_MUTANT)
		 * 		- Generate the crossover individuals in the range [@ref N_MUTANT, @ref pop_size)
		 * - Find the individual with the largest fit and decdode the chromosome.
		 *   Store the result in @e p. Store the cost of the solution in @e c.
		 * 
		 * @pre @p e is an empty instance of the problem, and @e c its cost.
		 */
		bool execute_algorithm(problem *p, double& c);
		
		/**
		 * @brief Prints the performance of the algorithm.
		 * 
		 * Prints to standard output a message with the following format:
		\verbatim
		RKGA algorithm performance:
			Total generation average:          DOUBLE s
			Average generation average:        DOUBLE s
			Total mutant generation time:      DOUBLE s
			Average mutant generation time:    DOUBLE s
			Total crossover generation time:   DOUBLE s
			Average crossover generation time: DOUBLE s
		\endverbatim
		 */
		void print_performance() const;
};

} // -- namespace algorithms
} // -- namespace metaheuristics

