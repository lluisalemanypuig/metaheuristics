#pragma once

// metaheuristic includes
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace algorithms {
using namespace structures;
using namespace random;

/// Interface for the two functions common to all algorithms.
template<
	class G = default_random_engine,
	typename dT = size_t
>
class metaheuristic {
	protected:
	public:
		/// Default constructor.
		metaheuristic();
		/// Destructor.
		virtual ~metaheuristic();
		
		// MODIFIERS
		
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

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/metaheuristic.hpp>
