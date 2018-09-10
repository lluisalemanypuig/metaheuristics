#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <iostream>
#include <utility>
#include <vector>
using namespace std;

// C++ includes
#include <limits>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/structures/infeasible_exception.hpp>
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
 * @brief Typedef for the population set.
 * 
 * Basically, a shorthand for the vector that contains elements
 * of type @ref structures::individual and uses an allocator specially
 * for that class (see @ref structures::allocator_individual).
 */
typedef vector<individual, allocator_individual<individual> > population_set;

/**
 * @brief Abstract class to implement genetic algorithms.
 * 
 * In this context, a "mutant individual" is an individual whose
 * chromosome has been randomly generated.
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
 * @param dT Type of the numbers generated by the discrete random generator (see metaheuristics::drng).
 * @param cT Type of the numbers generated by the continuous random generator (see metaheuristics::crng).
 */
template<
	class G = default_random_engine,
	typename dT = size_t,
	typename cT = double
>
class genetic_algorithm : public metaheuristic<G,dT> {
	private:
	protected:
		/// Continuous random generator for values between 0 and 1
		crandom_generator<G,cT> zero_one_rng;
		/// Discrete random generator for values in [0, @ref pop_size)
		drandom_generator<G,dT> population_rng;
		
		/// Total execution of the algorithm.
		double total_time;
		/**
		 * @brief Time needed to initialise the population.
		 * 
		 * Usually, this is just the time needed for allocating the
		 * memory, generating a random chromosome for each individual
		 * and evaluating the individuals.
		 */
		double initial_time;
		/// Time spent on doing the crossover of individuals.
		double crossover_time;
		/// Time spent in generating mutant individuals.
		double mutant_time;
		
		/// Size of the population.
		size_t pop_size;
		/// Size of the mutant population.
		size_t N_MUTANT;
		/// Number of generations of the population.
		size_t N_GEN;
		/// Size of the chromosomes of each individual.
		size_t chrom_size;
		/// Probability of inheritance.
		double in_prob;
		
		/// The population of individuals.
		population_set population;
		
		// Information display functions
		/// 
		/**
		 * @brief Prints to standard output the current population.
		 * 
		 * Each line contains the index of the individual within the
		 * population (0,1,2,3,...), its fitness and its chromosome.
		 */
		void print_current_population() const;
		
		// Algorithm-related functions
		/// Fills the population with mutants and decodes them.
		void initialize_population(const problem<G,dT> *p);
		/**
		 * @brief The individuals within the range [A,B) are replaced with mutants.
		 * @param[in] p The problem<G,dT> for which a solution will be generated with each chromosome.
		 * @param[in] A Lower bound of interval.
		 * @param[in] B Upper bound of interval.
		 * @param[out] next_gen The next generation of individuals, created
		 * from @ref population. The individuals within the range [@e A,@e B)
		 * of this set are modified.
		 * @param[out] m At the end of the execution @e m equals @e B.
		 */
		void generate_mutants(const problem<G,dT> *p, size_t A, size_t B, population_set& next_gen, size_t& m);
		
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
		 * @param[in] p The problem<G,dT> for which a solution will be generated
		 * with each chromosome.
		 * @param[out] next_gen The next generation of individuals, created
		 * from @ref population. The individuals within the range
		 * [m, @ref pop_size) are modified.
		 * @param[out] m At the end of the execution @e m equals @ref pop_size.
		 */
		void generate_crossovers(const problem<G,dT> *p, population_set& next_gen, size_t& m);
		
		/**
		 * @brief Evaluates an individual.
		 * 
		 * Creates a solution using method problem<G,dT>::decode and @e i's
		 * chromosome. Then sets @e i's fitness using the cost of that
		 * solution.
		 * 
		 * @param[in] p The problem<G,dT> for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[out] i Inidividual from which the solution is generated.
		 * @pre Inidividual @e i has its chromosome set.
		 * @post Inidividual @e i is given its fitness.
		 */
		void evaluate_individual(const problem<G,dT> *p, individual& i) const;
		/**
		 * @brief Generates a mutant individual.
		 * 
		 * Generates its random chromosome, and evaluates the individual
		 * (see @ref evaluate_individual(const problem<G,dT>*, individual&)const.
		 * @param[in] p The problem<G,dT> for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[out] i Inidividual from which the solution is generated.
		 * @post Inidividual @e i is given its chromosome and fitness.
		 */
		void generate_mutant(const problem<G,dT> *p, individual& i);
		
		/**
		 * @brief Makes the crossover of @e i-th and @e j-th individuals.
		 * 
		 * @e son is the result of making the crossover of the @e i-th
		 * and @e j-th individuals. With probability @ref in_prob,
		 * @e son will inherit @e i's gene.
		 * 
		 * @param[in] p The problem<G,dT> for which a solution will be generated
		 * with @e i's chromosome.
		 * @param[in] i First parent.
		 * @param[in] j Second parent.
		 * @param[out] son The result of the crossover of @e i-th and @e j-th
		 * individuals.
		 */
		void crossover(const problem<G,dT> *p, size_t i, size_t j, individual& son);
		
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
		
	public:
		/// Default constructor.
		genetic_algorithm(); 
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
		virtual ~genetic_algorithm();
		
		/// Returns a constant reference to the individual with largest fitness.
		virtual const individual& get_best_individual() const;
		/**
		 * @brief Returns a constant reference to the i-th individual.
		 * @pre @e i < @ref pop_size.
		 */
		const individual& get_individual(size_t i) const;
		
		/**
		 * @brief Returns the size of the population.
		 * @returns Returns the value of @ref pop_size.
		 */
		size_t population_size() const;
		
		/**
		 * @brief Executes a genetic algorithm.
		 * @param[out] p The solution obtained using the genetic algorithm.
		 * @param[out] c The cost of solution obtained using the genetic algorithm.
		 * @pre @e p contains an empty instance of the problem<G,dT>.
		 * @e c is the cost of @e p (the cost of an empty solution).
		 */
		virtual bool execute_algorithm(problem<G,dT> *p, double& c) = 0;
		
		/// Prints a summary of the performance of the algorithm
		virtual void print_performance() const = 0;
};

/// Shorthand for the genetic_algorithm class template
template<class G, typename dT, typename cT>
using GA = genetic_algorithm<G,dT,cT>;

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.cpp>
