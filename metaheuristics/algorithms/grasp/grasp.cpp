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

#include <metaheuristics/algorithms/grasp/grasp.hpp>

// C++ includes
#include <limits>
#include <iostream>

#if defined (GRASP_VERBOSE)
#include <iomanip>
#endif

// metaheuristics includes
#if defined (GRASP_DEBUG)
#include <metaheuristics/macros.hpp>
#endif

namespace metaheuristics {
namespace algorithms {

// PRIVATE

// PUBLIC

template<class G>
grasp<G>::grasp
(
	size_t mg, size_t ml, double a,
	const structures::local_search_policy& lsp
)
: metaheuristic<G>()
{
	MAX_ITER_GRASP = mg;
	MAX_ITER_LOCAL = ml;
	alpha = a;
	LSP = lsp;
}

// SETTERS

template<class G>
void grasp<G>::set_local_search_policy(const structures::local_search_policy& lsp) {
	LSP = lsp;
}

template<class G>
void grasp<G>::set_max_iterations_grasp(size_t max) {
	MAX_ITER_GRASP = max;
}

template<class G>
void grasp<G>::set_max_iterations_local(size_t max) {
	MAX_ITER_LOCAL = max;
}

template<class G>
void grasp<G>::reset_algorithm() {
	total_time = 0.0;
	construct_time = 0.0;
	local_search_time = 0.0;
	
	if (META<G>::seed_rng) {
		drng.seed_random_engine();
	}
}

// GETTERS

template<class G>
double grasp<G>::get_total_time() const {
	return total_time;
}

template<class G>
double grasp<G>::get_construct_time() const {
	return construct_time;
}

template<class G>
double grasp<G>::get_local_search_time() const {
	return local_search_time;
}

template<class G>
structures::local_search_policy grasp<G>::get_local_search_policy() const {
	return LSP;
}

template<class G>
size_t grasp<G>::get_max_iterations_grasp() const {
	return MAX_ITER_GRASP;
}

template<class G>
size_t grasp<G>::get_max_iterations_local() const {
	return MAX_ITER_LOCAL;
}

template<class G>
bool grasp<G>::execute_algorithm(structures::problem<G> *best, double& current_best_f) {
	// set algorithm to its initial state
	reset_algorithm();
	
	// timing variables
	timing::time_point bbegin, bend, begin, end;
	
	local_search<G> ls(MAX_ITER_LOCAL, LSP);
	current_best_f = -std::numeric_limits<double>::max();
	
	#if defined (GRASP_VERBOSE)
	std::cout
		<< setw(8)  << "GRASP"
		<< setw(15) << " "
		<< setw(18) << "Elaps. Time (s)"
		<< setw(18) << "Obj. Function"
		<< setw(12) << "Iter./" << MAX_ITER_GRASP
		<< std::endl;
	#endif
	
	bbegin = timing::now();
	for (size_t it = 1; it <= MAX_ITER_GRASP; ++it) {
		structures::problem<G> *r = best->empty();
		
		try {
			begin = timing::now();
			double rcc = r->random_construct(&drng, alpha);
			end = timing::now();
			construct_time += timing::elapsed_seconds(begin, end);
			
			#if defined (GRASP_DEBUG)
			if (not r->sanity_check(std::cerr)) {
				std::cerr << MH_ERROR << std::endl;
				std::cerr << "    Sanity check failed on solution returned by 'random_construct'." << std::endl;
				r->print(std::cerr, "");
			}
			#endif
			
			#if defined (GRASP_VERBOSE)
			if (rcc > current_best_f) {
				std::cout << setw(8) << "**R.C.";
			}
			else {
				std::cout << setw(8) << "R.C.";
			}

			std::cout
				<< setw(15) << " "
				<< setw(18) << timing::elapsed_seconds(bbegin, timing::now())
				<< setw(18) << rcc
				<< setw(12) << it
				<< std::endl;
			#endif
			
			double lsc = rcc;
			begin = timing::now();
			ls.execute_algorithm(r, lsc);
			end = timing::now();
			local_search_time += timing::elapsed_seconds(begin, end);
			
			#if defined (GRASP_DEBUG)
			if (not r->sanity_check(std::cerr)) {
				std::cerr << MH_ERROR << std::endl;
				std::cerr << "Sanity check failed on solution returned by local search algorithm." << std::endl;
				r->print(std::cerr, "");
			}
			#endif
			
			#if defined (GRASP_VERBOSE)
			if (lsc > rcc and lsc != numeric_limits<double>::max()) {
				std::cout << setw(8) << " ";

				if (lsc > current_best_f) {
					std::cout << setw(15) << "**L.S.";
				}
				else {
					std::cout << setw(15) << "L.S.";
				}

				std::cout
					<< setw(18) << timing::elapsed_seconds(bbegin, timing::now())
					<< setw(18) << lsc
					<< setw(12) << it
					<< std::endl;
			}
			#endif
			
			if (lsc > current_best_f) {
				current_best_f = lsc;
				best->copy(r);
			}
		}
		catch (const structures::infeasible_exception& e) {
			end = timing::now();
			construct_time += timing::elapsed_seconds(begin, end);
			
			#if defined (GRASP_VERBOSE)
			std::cout
				<< setw(8)  << "R.C."
				<< setw(15) << " "
				<< setw(18) << timing::elapsed_seconds(bbegin, timing::now())
				<< setw(18) << -1
				<< setw(12) << it
				<< std::endl;

			std::cout
				<< setw(8)  << " "
				<< setw(15) << "L.S."
				<< setw(18) << timing::elapsed_seconds(bbegin, timing::now())
				<< setw(18) << -1
				<< setw(12) << it
				<< std::endl;
			#endif
		}
		
		delete r;
	}
	bend = timing::now();
	total_time = timing::elapsed_seconds(bbegin, bend);
	
	return true;
}

template<class G>
void grasp<G>::print_performance() const {
	std::cout << "GRASP metaheuristic performance (for a total of " << MAX_ITER_GRASP << " iterations):" << std::endl;
	std::cout << "    Total execution time:      " << total_time << " s" << std::endl;
	std::cout << "    Average iteration time:    " << total_time/MAX_ITER_GRASP << " s" << std::endl;
	std::cout << "    Total construction time:   " << construct_time << " s" << std::endl;
	std::cout << "    Average construction time: " << construct_time/MAX_ITER_GRASP << " s" << std::endl;
	std::cout << "    Total local search time:	" << local_search_time << " s" << std::endl;
	std::cout << "    Average local search time:	" << local_search_time/MAX_ITER_GRASP << " s" << std::endl;
	std::cout << std::endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

