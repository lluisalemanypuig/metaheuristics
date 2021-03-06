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
#include <random>

// metaheursitics includes
#include <metaheuristics/algorithms/metaheuristic.hpp>
#include <metaheuristics/structures/policies.hpp>
#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace algorithms {

/**
 * @brief Local Search heuristic algorithm.
 * 
 * Given a maximum number of iterations and a local search policy
 * (see @ref metaheuristics::structures::local_search_policy) apply
 * the local search algorithm on an instance of a problem.
 * 
 * Given an instance of a problem @e p, with cost @e c,
 * this algorithm works as follows: 
 * - Define BEST = @e p
 * - Apply while there is improvement and for a maximum number
 * 		of iterations:
 * 		- Find the best solution among those in its neighbourhood,
 * 		according to the search policy.
 * 		- Define BEST to be that solution
 * - Return BEST
 * 
 * The number of iterations can be set in the constructor
 * (see @ref local_search(size_t, const local_search_policy&))
 * or in the method @ref set_max_iterations(size_t).
 * 
 * Call @ref execute_algorithm function to execute this algorithm.
 * 
 * Compile the library with the flag
	\verbatim
	LOCAL_SEARCH_VERBOSE
	\endverbatim
 * to see the progress of the algorithm.
 * 
 * In addition to the previous flag, one can also compile the library
 * with the flag
	\verbatim
	LOCAL_SEARCH_DEBUG
	\endverbatim
 * to run the sanity check on every "best neighbour" found.
 * 
 * This class needs the implementation of the following methods:
 * - @ref problem::best_neighbour
 * - @ref problem::copy
 * 
 * In case the flag LOCAL_SEARCH_DEBUG is defined then these other
 * methods are also required:
 * - @ref problem::sanity_check
 * - @ref problem::print
 * 
 * @param G The type of the engine of the random generator (see metaheuristics::drng).
 */
template<
	class G = std::default_random_engine
>
class local_search : public metaheuristic<G> {
	public:
		/// Default constructor.
		local_search() = default;
		/// 
		/**
		 * @brief Constructor with maximum number of iterations and search policy.
		 * @param m Maximum number of iterations of the algorithm.
		 * @param lsp Local Search policy (see
		 * @ref metaheuristics::structures::local_search_policy).
		 */
		local_search(size_t m, const structures::local_search_policy& lsp);
		/// Destructor
		~local_search() = default;
		
		// SETTERS
		
		/**
		 * @brief Sets the local search policy.
		 * 
		 * Sets the value of @ref LSP to @e lsp.
		 */
		void set_local_search_policy(const structures::local_search_policy& lsp);
		/**
		 * @brief Sets the maximum number of iterations of the algorithm.
		 * 
		 * Sets the value of @ref MAX_ITER to @e m.
		 */
		void set_max_iterations(size_t m);
		/**
		 * @brief Resets the algorithm to its initial state.
		 * 
		 * Sets to 0 the internal variables @ref total_time, @ref neighbourhood_time,
		 * @ref MAX_ITER, @ref ITERATION. The policy @ref LSP is not
		 * modified.
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
		 * @brief Returns the execution time needed to explore the neighbourhood.
		 * @returns Returns the value that @ref neighbourhood_time has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		double get_neighbourhood_time() const;
		/**
		 * @brief Returns the execution time needed to explore the neighbourhood.
		 * @returns Returns the value the variable @ref ITERATION has at
		 * the end of the execution of @ref execute_algorithm().
		 */
		size_t get_n_iterations() const;
		/**
		 * @brief Returns the policy of the search.
		 * @returns Returns the value of @ref LSP.
		 */
		structures::local_search_policy get_local_search_policy() const;
		/**
		 * @brief Returns the number of iterations.
		 * @returns Returns the value of @ref MAX_ITER.
		 */
		size_t get_max_iterations() const;
		
		/**
		 * @brief Execute the local search algorithm.
		 * 
		 * @param[in] p The instance of the problem.
		 * @param[in] c The cost of the instance @e p at the beginning of
		 * the execution.
		 * @param[out] p Whence the algorithm has finished @e p contains
		 * an improved solution to the problem.
		 * @param[out] c The cost of the solution stored at @e p at the
		 * end of the execution of the algorithm.
		 */
		bool execute_algorithm(structures::problem<G> *p, double& c);
		
		/**
		 * @brief Prints a summary of the performance of the algorithm.
		 * 
		 * Prints to standard output a message with the following format:
		\verbatim
		Local Search algorithm performance
		    Number of iterations:                   INT
		    Total execution time:                   DOUBLE s
		    Average iteration time:                 DOUBLE s
		    Total neihgbourhood exploration time:   DOUBLE s
		    Average neihgbourhood exploration time: DOUBLE s
		\endverbatim
		 */
		void print_performance() const;

	private:
		/// Total execution time of the algorithm.
		double total_time = 0;
		/// Total execution time to explore the different neighbourhoods.
		double neighbourhood_time = 0;

		/// Maximum number of iterations.
		size_t MAX_ITER = -1;
		/// Current iteration of the algorithm.
		size_t ITERATION = 1;
		/// Policy of the local search.
		structures::local_search_policy LSP;
};

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/local_search/local_search.cpp>
