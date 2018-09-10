#pragma once

// metaheuristic includes
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random;

/**
 * @brief Interface for the two functions common to all algorithms.
 * 
 * Common variables are:
 * - @ref seed_rng
 * 
 * The methods to implement for are:
 * - @ref reset_algorithm
 * - @ref execute_algorithm
 */
template<
	class G = default_random_engine,
	typename dT = size_t
>
class metaheuristic {
	protected:
		/// Variable telling whether the number generators should be seeded or not.
		bool seed_rng;
		
	public:
		/**
		 * @brief Default constructor.
		 * 
		 * Variable @ref seed_rng is initialised to @e false.
		 */
		metaheuristic();
		/// Destructor.
		virtual ~metaheuristic();
		
		// MODIFIERS
		
		/**
		 * @brief Seed the random number generators.
		 * 
		 * Sets variable @ref seed_rng to true.
		 */
		void seed();
		
		/**
		 * @brief Sets the algorithm to its original state.
		 * 
		 * For example, if the algorithm has variables storing the
		 * number of iterations, execution time, ... here is where
		 * one would set all these variables to 0.
		 */
		virtual void reset_algorithm() = 0;
		
		/**
		 * @brief Starts the execution of the algorithm.
		 * @returns Returns true if it terminates succesfully. Returns
		 * false if otherwise.
		 */
		virtual bool execute_algorithm(problem<G,dT> *p, double& c) = 0;
};

/// Shorthand for the genetic_algorithm class template
template<class G, typename dT>
using META = metaheuristic<G,dT>;

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/metaheuristic.cpp>
