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
	typename dT = size_t,
	typename cT = double
>
class metaheuristic {
	protected:
		/// Discrete random number generator.
		drandom_generator<G,dT> *drng;
		/// Continuous random number generator.
		crandom_generator<G,cT> *crng;
	
	public:
		/// Default constructor.
		metaheuristic();
		/// Constructor with random generator.
		metaheuristic(drandom_generator<G,dT> *drng, crandom_generator<G,cT> *crng);
		/// Destructor.
		virtual ~metaheuristic();
		
		// MODIFIERS
		
		/**
		 * @brief Sets the discrete random number generator.
		 * 
		 * The caller is made responsible for freeing this object.
		 */
		void set_drng(drandom_generator<G,dT> *rng);
		/**
		 * @brief Sets the discrete random number generator.
		 * 
		 * The caller is made responsible for freeing this object.
		 */
		void set_crng(crandom_generator<G,cT> *rng);
		
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

