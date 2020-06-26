/*********************************************************************
 *
 * Collection of heuristic and metaheuristic algorithms
 *
 * Copyright (C) 2018-2020  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 *********************************************************************/

#pragma once

// C++ includes
#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>
#include <random>
using namespace std;

// C++ includes
#include <limits>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.hpp>
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/structures/chromosome.hpp>
#include <metaheuristics/structures/individual.hpp>
#include <metaheuristics/structures/problem.hpp>
#include <metaheuristics/misc/time.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random;
using namespace timing;

/**
 * @brief Implementation of the RKGA.
 * 
 * RKGA stands for Random-Key Genetic Algorithm.
 * 
 * Given an instance of a problem @e p, with cost @e c,
 * the algorithm implemented in @ref execute_algorithm(problem<G>*, double&)
 * roughly follows the following pseudocode:
 * - Initialise the population with @ref genetic_algorithm::pop_size mutants
 * - For as many generations as @ref genetic_algorithm::N_GEN :
 * 		- Make the next generation @e Ng
 * 		- Generate mutants in the range [0, @ref genetic_algorithm::N_MUTANT) in @e Ng
 * 		- Generate the crossover individuals in the range
 * 		  [@ref genetic_algorithm::N_MUTANT, @ref genetic_algorithm::pop_size)
 * 		  in @e Ng
 * - Find the individual with the largest fit and decdode the chromosome.
 *   Store the result in @e p. Store the cost of the solution in @e c.
 * 
 * A more complete description of this algorithm can be found in this paper:
 \verbatim
 Biased random-key genetic algorithms for combinatorial optimization
 Gonçalves, José Fernando and Resende, Mauricio G. C.
 1995, Journal of Heuristics, Volume 17, Number 5
 ISSN: 1381-1231
 \endverbatim
 * 
 * In this algorithm the @ref genetic_algorithm::population is split into
 * two groups:
 * - The individuals within the range [0, @ref genetic_algorithm::N_MUTANT),
 * are mutant individuals.
 * -  The rest of @ref genetic_algorithm::population (range
 * [ @ref genetic_algorithm::N_MUTANT, @ref genetic_algorithm::pop_size)),
 * are crossover individuals.
 * 
 * This algorithm needs the implementation of the following methods:
 * - @ref problem::clone
 * - @ref problem::decode
 * 
 * See @ref genetic_algorithm for information on compilation flags.
 * 
 * @param G The type of the engine of the random generator (see metaheuristics::drng).
 */
template<
	class G = default_random_engine
>
class rkga : public genetic_algorithm<G> {
	private:
		/**
		 * @brief Implementation of the parents choice.
		 * 
		 * Choose two individuals at random from @ref genetic_algorithm::population.
		 * @e p1 and @e p2 are two indices within the range [0, @ref genetic_algorithm::pop_size).
		 */
		void get_two_parents(size_t& p1, size_t& p2);
		
	protected:
		
		// Sanity check
		/**
		 * @brief Makes sure that the different populations sizes are correct.
		 * 
		 * Checks that @ref genetic_algorithm::N_MUTANT < @ref genetic_algorithm::pop_size. Basically, a sanity check.
		 */
		bool are_set_sizes_correct() const;
		
	public:
		/// Default constructor
		rkga();
		/**
		 * @brief Constructor with parameters.
		 * @param p_size The total size of the population (see @ref genetic_algorithm::pop_size).
		 * @param m_size The amount of mutant individuals (see @ref genetic_algorithm::N_MUTANT).
		 * @param n_gen The number of generations (see @ref genetic_algorithm::N_GEN).
		 * @param chrom_size The size of each individual's chromosome (see @ref chrom_size).
		 * @param i_prob Inheritance probability (see @ref genetic_algorithm::in_prob).
		 */
		rkga(size_t p_size, size_t m_size, size_t n_gen, size_t chrom_size, double i_prob);
		/// Destructor.
		virtual ~rkga();
		
		/**
		 * @brief Sets the internal variables to its initial state.
		 * 
		 * Calls @ref genetic_algorithm::reset_genetic_algorithm.
		 */
		void reset_algorithm();
		
		/**
		 * @brief Executes the RKGA algorithm.
		 * 
		 * @param[in] p The instance of the problem.
		 * @param[in] c The cost of the instance @e p at the beginning of
		 * the execution.
		 * @param[out] p Whence the algorithm has finished @e p contains
		 * an improved solution to the problem.
		 * @param[out] c The cost of the solution stored at @e p at the
		 * end of the execution of the algorithm.
		 */
		bool execute_algorithm(problem<G> *p, double& c);
		
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

#include <metaheuristics/algorithms/genetic_algorithms/rkga.cpp>
