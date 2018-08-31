#pragma once

// C++ includes
#include <iostream>
#include <iomanip>
using namespace std;

// Custom includes
#include "algorithms/metaheuristic.hpp"
#include "algorithms/problem.hpp"
#include "misc/timing.hpp"

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace mtime;

/**
 * @brief Local Search heuristic algorithm
 * 
 * Given a maximum number of iterations and a local search policy
 * (see @ref metaheuristics::structures::local_search_policy) apply
 * the local search algorithm on an instance of a problem.
 * 
 * See @ref execute_algorithm for details on how this
 * algorithm works.
 * 
 * Compile the library with the flag
	\verbatim
	LOCAL_SEARCH_VERBOSE
	\endverbatim
 * to see the progress of the algorithm.
 */
class local_search : public metaheuristic {
	private:
		/// Total execution time of the algorithm
		double total_time;
		/// Total execution time to explore the different neighbourhoods
		double neighbourhood_time;
		
		/// Maximum number of iterations
		size_t MAX_ITER;
		/// Current iteration of the algorithm
		size_t ITERATION;
		/// Policy of the local search
		local_search_policy LSP;
				
	public:
		/// Default constructor
		local_search();
		/// 
		/**
		 * @brief Constructor with maximum number of iterations and search policy
		 * @param MAX_ITER Maximum number of iterations of the algorithm
		 * @param lsp Local Search policy
		 * (see @ref metaheuristics::structures::local_search_policy)
		 */
		local_search(size_t MAX_ITER, const local_search_policy& lsp);
		/// Destructor
		~local_search();
		
		// SETTERS
		
		/// Sets the local search policy
		void set_local_search_policy(const local_search_policy& lsp);
		/// Sets the maximum number of iterations of the algorithm
		void set_max_iterations(size_t MAX_ITER);
		/**
		 * @brief Resets the algorithm to its initial state
		 * 
		 * Sets to 0 the internal variables @ref total_time, @ref neighbourhood_time,
		 * @ref MAX_ITER, @ref ITERATION. The policy @ref LSP is left
		 * untouched.
		 */
		void reset_algorithm();
		
		// GETTERS
		
		/// Returns the total execution time of the algorithm
		double get_total_time() const;
		/// Returns the execution time needed to explore the neighbourhood
		double get_neighbourhood_time() const;
		/// Returns the number of iterations that the algorithm made
		size_t get_n_iterations() const;
		
		/// Returns the policy of the search
		local_search_policy get_local_search_policy() const;
		/// Returns the number of iterations
		size_t get_max_iterations() const;
		
		/**
		 * @brief Execute the local search algorithm
		 * 
		 * This algorithm works as follows:
		 * - Given an instance of a problem @e p, with cost @e c
		 * - Define BEST = @e p
		 * - Apply while there is improvement and for a maximum number of
		 * 		of iterations:
		 * 		- 3.1. Find the best solution among those in its neighbourhood,
		 * 		according to the search policy.
		 * 		- 3.2. Define BEST to be that solution
		 * - Return BEST
		 * 
		 * The number of iterations can be set in the constructor
		 * (see @ref local_search(size_t, const local_search_policy&))
		 * or in the method @ref set_max_iterations(size_t)
		 * 
		 * @param[in] p The instance of the problem.
		 * @param[in] c The cost of the instance p at the beginning of
		 * the execution
		 * @param[out] p Whence the algorithm has finished p contains
		 * an improved solution to the problem
		 * 
		 * @pre The method @ref reset_algorithm does not need to be called
		 */
		bool execute_algorithm(problem *p, double& c);
		
		/**
		 * @brief Prints a summary of the performance of the algorithm
		 * 
		 * Prints to standard output a message with the following format:
		\verbatim
		Local Search algorithm performance
		    Number of iterations:                   INT
		    Total execution time:                   DOUBLE
		    Average iteration time:                 DOUBLE
		    Total neihgbourhood exploration time:   DOUBLE
		    Average neihgbourhood exploration time: DOUBLE
		\endverbatim
		 */
		void print_performance() const;
};

}
}

