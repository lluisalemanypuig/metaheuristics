/*********************************************************************
 * Collection of heuristic and metaheuristic algorithms
 * Copyright (C) 2018  Lluís Alemany Puig
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
#include <iomanip>
#include <random>
#include <limits>
using namespace std;

// Custom includes
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/algorithms/local_search/local_search.hpp>
#include <metaheuristics/random/random_generator.hpp>
#include <metaheuristics/structures/policies.hpp>
#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace algorithms {

using namespace structures;
using namespace random;
using namespace timing;

/**
 * @brief GRASP metaheuristic algorithm.
 * 
 * Implementation of the Greedy Randomised Adaptative-Search Procedure.
 * 
 * Given an instance of a problem @e p, with cost @e c,
 * the algorithm implemented in @ref execute_algorithm(problem<G>*, double&)
 * roughly follows the following pseudocode:
 * - Define BEST an empty solution, using object @e p
 * - For as many iterations as @ref MAX_ITER_GRASP
 * - Apply while there is improvement and for a maximum number
 * 		of iterations:
 * 		- Construct a random solution with a Restricted
 * 		candidate list built using parameter @ref alpha.
 * 		Define R as the result.
 * 		- Apply the local search procedure on R for at most
 * 		@ref MAX_ITER_LOCAL with policy @ref LSP. Define L as
 * 		the result.
 * 		- Keep the best solution between BEST and L.
 * - Return BEST
 * 
 * The number of iterations can be set in the constructor
 * (see @ref grasp(size_t,size_t,double,const local_search_policy&))
 * or in the methods @ref set_max_iterations_grasp(size_t),
 * @ref set_max_iterations_local(size_t).
 * 
 * A more complete description of this algorithm can be found in this paper:
 \verbatim
 Greedy Randomized Adaptive Search Procedures
 Feo, Thomas A. and Resende, Mauricio G. C.
 1995, Journal of Global Optimization, Volume 6, Number 2
 ISSN: 1573-2916
 \endverbatim
 * 
 * Call @ref execute_algorithm function to execute this algorithm.
 * 
 * Compile the library with the flag
	\verbatim
	GRASP_VERBOSE
	\endverbatim
 * to see the progress of the algorithm.
 * 
 * In addition to the previous flag, one can also compile the library
 * with the flag
	\verbatim
	GRASP_DEBUG
	\endverbatim
 * to run the sanity check on every solution constructed randomly,
 * and on every solution found by the local search procedure.
 * 
 * This algorithm needs the implementation of the following methods:
 * - @ref problem::empty
 * - @ref problem::random_construct
 * - @ref problem::copy
 * 
 * In case the flag GRASP_DEBUG is defined then these other
 * methods are also required:
 * - @ref problem::sanity_check
 * - @ref problem::print
 * 
 * The local search procedure also has its own compilation flags.
 * See @ref local_search for details.
 * @param G The type of the engine of the random generator (see metaheuristics::drng).
 */
template<
	class G = default_random_engine
>
class grasp : public metaheuristic<G> {
	private:
		/// Discrete random number generator
		drandom_generator<G,size_t> drng;
		
		/// Total execution time of the algorithm.
		double total_time;
		/// Total execution time spent in constructing the initial
		/// randomised solution.
		double construct_time;
		/// Total execution time spent in the Local Search.
		double local_search_time;
		
		/// Maximum number of iterations of the GRASP algorithm.
		size_t MAX_ITER_GRASP;
		/// Maximum number of iterations of the Local Search procedure.
		size_t MAX_ITER_LOCAL;
		/// Parameter used to construct the Restricted Candidate List.
		double alpha;
		/// Policy for the Local Search procedure.
		local_search_policy LSP;
		
	public:
		/// Default constructor.
		grasp();
		/// Constructor with several parameters.
		grasp
		(
			size_t m_GRASP, size_t m_LOCAL, double a,
			const local_search_policy& lsp
		);
		/// Destructor.
		~grasp();
		
		// SETTERS
		
		/**
		 * @brief Sets the policy for the Local Search procedure.
		 * 
		 * Sets the value of @ref LSP to @e lsp.
		 */
		void set_local_search_policy(const local_search_policy& lsp); 
		/**
		 * @brief Sets the maximum number of iterations of the GRASP algorithm.
		 * 
		 * Sets the value of @ref MAX_ITER_GRASP to @e max.
		 */
		void set_max_iterations_grasp(size_t max);
		/**
		 * @brief Sets the maximum number of iterations of the Local Search procedure.
		 * 
		 * Sets the value of @ref MAX_ITER_LOCAL to @e max.
		 */
		void set_max_iterations_local(size_t max);
		/**
		 * @brief Resets the algorithm to its initial state.
		 * 
		 * Sets to 0 the internal variables @ref total_time, @ref construct_time,
		 * @ref local_search_time. Seeds the random number generator @ref drng.
		 * 
		 * The other variables (@ref MAX_ITER_GRASP,
		 * @ref MAX_ITER_LOCAL, @ref alpha, @ref LSP) are not modified.
		 */
		void reset_algorithm();
		
		// GETTERS
		
		/**
		 * @brief Returns the total execution time of the algorithm.
		 * @returns Returns the value that @ref total_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_total_time() const;
		/**
		 * @brief Returns the total time needed for the construction of random solutions.
		 * @returns Returns the value that @ref construct_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_construct_time() const;
		/**
		 * @brief Returns the total time spent in the Local Search procedure.
		 * @returns Returns the value that @ref local_search_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_local_search_time() const;
		
		/**
		 * @brief Returns the policy of the search.
		 * @returns Returns the value of @ref LSP.
		 */
		local_search_policy get_local_search_policy() const;
		/**
		 * @brief Returns the maximum number of iterations of the GRASP algorithm.
		 * @returns Returns the value of @ref MAX_ITER_GRASP.
		 */
		size_t get_max_iterations_grasp() const;
		/**
		 * @brief Returns the maximum number of iterations of the Local Search procedure.
		 * @returns Returns the value of @ref MAX_ITER_LOCAL.
		 */
		size_t get_max_iterations_local() const;
		
		/**
		 * @brief Execute the GRASP algorithm.
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
		 * @brief Prints a summary of the performance of the algorithm.
		 * 
		 * Prints to standard output a message with the following format:
		\verbatim
		GRASP metaheuristic performance (for a total of INT iterations):
			Total execution time:      DOUBLE s
			Average iteration time:    DOUBLE s
			Total construction time:   DOUBLE s
			Average construction time: DOUBLE s
			Total local search time:   DOUBLE s
			Average local search time: DOUBLE s
		\endverbatim
		 */
		void print_performance() const;
};

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/grasp/grasp.cpp>
