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
#include <vector>

// metaheursitics includes
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/structures/infeasible_exception.hpp>
#include <metaheuristics/structures/chromosome.hpp>
#include <metaheuristics/structures/individual.hpp>
#include <metaheuristics/structures/problem.hpp>
#include <metaheuristics/misc/time.hpp>

namespace metaheuristics {
namespace algorithms {

/**
 * @brief Typedef for the population set.
 * 
 * Basically, a shorthand for the vector that contains elements
 * of type @ref structures::individual and uses an allocator specially
 * for that class (see @ref structures::allocator_individual).
 */
typedef
	std::vector<
		structures::individual,
		structures::allocator_individual<structures::individual>
	> population_set;

/**
 * @brief Abstract class to implement genetic algorithms.
 * 
 * Abstract interface for two well-known Genetic Algorithms:
 * - RKGA
 * - BRKGA
 * 
 * In this context, a "mutant individual" is an individual whose
 * chromosomes (or alleles) has been randomly generated.
 * 
 * Compile the library with the flag
	\verbatim
	GENETICS_VERBOSE
	\endverbatim
 * to see the progress of the algorithm.
 * 
 * In addition to the previous flag, one can also compile the library
 * with the flag
	\verbatim
	GENETICS_DEBUG
	\endverbatim
 * to see the contents of the population (i.e., the genes of each individual).
 * 
 * @param G The type of the engine of the random generator (see metaheuristics::drng).
 */
template<
	class G = std::default_random_engine
>
class genetic_algorithm : public metaheuristic<G> {
	public:
		/// Default constructor.
		genetic_algorithm() = default;
		/**
		 * @brief Constructor with parameters.
		 * @param population_size The total size of the population
		 * (see @ref pop_size).
		 * @param mutant_population_size The size of the mutant population
		 * (see @ref N_MUTANT).
		 * @param num_generations The number of generations the algorithm
		 * will make (see @ref N_GEN).
		 * @param chrom_size The size of each inidivual's chromosome
		 * (see @ref chrom_size).
		 * @param inheritance_probability Inherith with such probability
		 * the first parent's gene (see @ref in_prob).
		 */
		genetic_algorithm
		(
			size_t population_size,
			size_t mutant_population_size,
			size_t num_generations,
			size_t chrom_size,
			double inheritance_probability
		);
		/// Destructor.
		virtual ~genetic_algorithm() = default;
		
		/// Returns a constant reference to the individual with largest fitness.
		virtual const structures::individual& get_best_individual() const;
		/**
		 * @brief Returns a constant reference to the i-th individual.
		 * @pre @e i < @ref pop_size.
		 */
		const structures::individual& get_individual(size_t i) const;
		
		/**
		 * @brief Returns the size of the population.
		 * @returns Returns the value of @ref pop_size.
		 */
		size_t population_size() const;
		
		/**
		 * @brief Executes a genetic algorithm.
		 * @param[out] p The solution obtained using the genetic algorithm.
		 * @param[out] c The cost of solution obtained using the genetic algorithm.
		 * @pre @e p contains an empty instance of the problem<G>.
		 * @e c is the cost of @e p (the cost of an empty solution).
		 */
		virtual bool execute_algorithm(structures::problem<G> *p, double& c) = 0;
		
		/// Prints a summary of the performance of the algorithm
		virtual void print_performance() const = 0;

	protected:
		/// Continuous random generator for values between 0 and 1
		random::crandom_generator<G,double> zero_one_rng;
		/// Discrete random generator for values in [0, @ref pop_size)
		random::drandom_generator<G,size_t> population_rng;

		/// Total execution of the algorithm.
		double total_time = 0.0;
		/**
		 * @brief Time needed to initialise the population.
		 *
		 * Usually, this is just the time needed for allocating the
		 * memory, generating a random chromosome for each individual
		 * and evaluating the individuals.
		 */
		double initial_time = 0.0;
		/// Time spent on doing the crossover of individuals.
		double crossover_time = 0.0;
		/// Time spent in generating mutant individuals.
		double mutant_time = 0.0;

		/// Size of the population.
		size_t pop_size = 0;
		/// Size of the mutant population.
		size_t N_MUTANT = 0;
		/// Number of generations of the population.
		size_t N_GEN = 0;
		/// Size of the chromosomes of each individual.
		size_t chrom_size = 0;
		/// Probability of inheritance.
		double in_prob = 1.0;

		/// The population of individuals.
		population_set population;

	protected:

		// Information-display functions
		///
		/**
		 * @brief Prints to standard output the current population.
		 *
		 * Each line contains the index of the individual within the
		 * population (0,1,2,3,...), its fitness and its chromosome.
		 */
		void print_current_population() const;

		// Algorithm-related functions
		/**
		 * @brief Fills the population with mutants and decodes them.
		 *
		 * Initialises each individual with its random chromosome.
		 * Then, decodes the chromosome and computes the fitness
		 * of the solution.
		 *
		 * The result of decoding the chromosome is freed and only
		 * the fitness is kept.
		 */
		void initialize_population(const structures::problem<G> *p);
		/**
		 * @brief The individuals within the range [A,B) are replaced with mutants.
		 * @param[in] p The problem<G> for which a solution will be generated with each chromosome.
		 * @param[in] A Lower bound of interval.
		 * @param[in] B Upper bound of interval.
		 * @param[out] next_gen The next generation of individuals, created
		 * from @ref population. The individuals within the range [@e A,@e B)
		 * of this set are modified.
		 * @param[out] m At the end of the execution @e m equals @e B.
		 */
		void generate_mutants(
			const structures::problem<G> *p, size_t A, size_t B,
			population_set& next_gen, size_t& m
		);

		/**
		 * @brief Choose two parents to make a crossover individual.
		 *
		 * With probability @ref in_prob, parent @e p1's gene is chosen
		 * over @e p2's gene when making a new individual.
		 *
		 * @param[out] p1 First parent. This parent is chosen over @e p2
		 * with probability @ref in_prob.
		 * @param[out] p2 Second parent.
		 */
		virtual void get_two_parents(size_t& p1, size_t& p2) = 0;
		/**
		 * @brief Generates the crossover population.
		 *
		 * @param[in] p The problem<G> for which a solution will be generated
		 * with each chromosome.
		 * @param[out] next_gen The next generation of individuals, created
		 * from @ref population. The individuals within the range
		 * [m, @ref pop_size) are modified.
		 * @param[out] m At the end of the execution @e m equals @ref pop_size.
		 */
		void generate_crossovers(
			const structures::problem<G> *p,
			population_set& next_gen, size_t& m
		);

		/**
		 * @brief Evaluates an individual.
		 *
		 * Creates a solution using method @ref problem<G>::decode and @e i's
		 * chromosome. Then sets @e i's fitness using the cost of that
		 * solution. The solution is finally freed.
		 *
		 * In case the method @ref problem<G>::decode returns an
		 * @ref structures::infeasible_exception the fitness is set to
		 * minus infinity.
		 *
		 * @param[in] p The problem<G> for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[out] i Inidividual from which the solution is generated.
		 * @pre Inidividual @e i has its chromosome set.
		 * @post Inidividual @e i is given its fitness.
		 */
		void evaluate_individual
		(const structures::problem<G> *p, structures::individual& i) const;
		/**
		 * @brief Generates a mutant individual.
		 *
		 * Generates its random chromosome, and evaluates the individual
		 * (see @ref evaluate_individual(const problem<G>*, individual&)const )
		 *
		 * @param[in] p The problem<G> for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[out] i Inidividual from which the solution is generated.
		 * @post Inidividual @e i is given its chromosome and fitness.
		 */
		void generate_mutant
		(const structures::problem<G> *p, structures::individual& i);

		/**
		 * @brief Makes the crossover of @e i-th and @e j-th individuals.
		 *
		 * @e son is the result of making the crossover of the @e i-th
		 * and @e j-th individuals. With probability @ref in_prob,
		 * @e son will inherit @e i's gene.
		 *
		 * @param[in] p The problem<G> for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[in] i First parent.
		 * @param[in] j Second parent.
		 * @param[out] child The result of the crossover of @e i-th and @e j-th
		 * individuals.
		 */
		void crossover(
			const structures::problem<G> *p, size_t i, size_t j,
			structures::individual& child
		);

		/**
		 * @brief Resets the genetic algorithm to a partial initial state.
		 *
		 * Sets the variables @ref total_time, @ref initial_time,
		 * @ref crossover_time, @ref mutant_time to 0.
		 *
		 * Seeds the random number generators @ref zero_one_rng and
		 * @ref population_rng.
		 */
		void reset_genetic_algorithm();

		/// Initialise the random generators to the appropriate ranges.
		void initialise_generators();
};

/// Shorthand for the genetic_algorithm class template
template<class G>
using GA = genetic_algorithm<G>;

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.cpp>
