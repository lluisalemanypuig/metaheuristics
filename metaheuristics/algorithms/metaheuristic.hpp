#pragma once

// metaheuristic includes
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/algorithms/problem.hpp>

namespace metaheuristics {
namespace algorithms {
using namespace structures;
using namespace random;

/// Interface for the two functions common to all algorithms
class metaheuristic {
	protected:
		/// Random number generator
		random_generator<> *rng;
	
	public:
		/// Default constructor
		metaheuristic();
		/// Constructor with random generator
		metaheuristic(random_generator<> *rng);
		/// Destructor
		virtual ~metaheuristic();
		
		/**
		 * @brief Sets the algorithm to its original state
		 * 
		 * For example, if the algorithm has variables storing the
		 * number of iterations, execution time, ... here is where
		 * one would set all these variables to 0.
		 */
		virtual void reset_algorithm() = 0;
		
		/**
		 * @brief Starts the execution of the algorithm
		 * @returns Returns true if it terminates succesfully. Returns
		 * false if otherwise.
		 */
		virtual bool execute_algorithm(problem *p, double& c) = 0;
};

}
}

