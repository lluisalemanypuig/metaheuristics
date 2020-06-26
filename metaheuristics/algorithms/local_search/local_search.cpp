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

#include <metaheuristics/algorithms/local_search/local_search.hpp>

// C++ includes
#include <iostream>
#if defined (LOCAL_SEARCH_VERBOSE)
#include <iomanip>
#endif

// metaheuristics includes
#include <metaheuristics/misc/time.hpp>
#if defined (LOCAL_SEARCH_DEBUG)
#include <metaheuristics/macros.hpp>
#endif

namespace metaheuristics {
namespace algorithms {

// PUBLIC

template<class G>
local_search<G>::local_search(
	size_t max, const structures::local_search_policy& lsp
)
: metaheuristic<G>()
{
	MAX_ITER = max;
	LSP = lsp;
	reset_algorithm();
}

// SETTERS

template<class G>
void local_search<G>::set_local_search_policy(const structures::local_search_policy& lsp) {
	LSP = lsp;
}

template<class G>
void local_search<G>::set_max_iterations(size_t max) {
	MAX_ITER = max;
}

template<class G>
void local_search<G>::reset_algorithm() {
	total_time = 0.0;
	neighbourhood_time = 0.0;
	ITERATION = 1;
}

// GETTERS

template<class G>
double local_search<G>::get_total_time() const {
	return total_time;
}

template<class G>
double local_search<G>::get_neighbourhood_time() const {
	return neighbourhood_time;
}

template<class G>
size_t local_search<G>::get_n_iterations() const {
	return ITERATION;
}

template<class G>
structures::local_search_policy local_search<G>::get_local_search_policy() const {
	return LSP;
}

template<class G>
size_t local_search<G>::get_max_iterations() const {
	return MAX_ITER;
}

template<class G>
bool local_search<G>::execute_algorithm(structures::problem<G> *best, double& current_best_f) {
	// set the algorithm to its initial state
	reset_algorithm();
	
	#if defined (LOCAL_SEARCH_VERBOSE)
	std::cout
		<< setw(8)  << " "
		<< setw(15) << "Local Search"
		<< setw(18) << "Elaps. Time (s)"
		<< setw(18) << "Obj. Function"
		<< setw(12) << "Iter./" << MAX_ITER
		<< setw(25) << "# Neighbours explored"
		<< std::endl;

	std::cout
		<< setw(8)  << " "
		<< setw(15) << " "
		<< setw(18) << 0.0
		<< setw(18) << current_best_f
		<< setw(12) << 0
		<< setw(25) << 0
		<< std::endl;
	#endif
	
	// 
	timing::time_point bbegin, bend, begin, end;
	
	bool improvement = true;
	
	bbegin = timing::now();
	while (ITERATION <= MAX_ITER and improvement) {
		std::pair<structures::problem<G>*, double> neighbour;
		neighbour.first = nullptr;
		neighbour.second = current_best_f;
		
		begin = timing::now();
		best->best_neighbour(neighbour, LSP);
		end = timing::now();
		neighbourhood_time += timing::elapsed_seconds(begin, end);
		
		if (neighbour.first != nullptr) {
			
			#if defined (LOCAL_SEARCH_DEBUG)
			if (not best->sanity_check(std::cerr)) {
				std::cerr << MH_ERROR << std::endl;
				std::cerr << "    Sanity check failed on solution returned by 'best_neighbour'." << std::endl;
				best->print(std::cerr, "");
			}
			#endif
			
			if (neighbour.second > current_best_f) {
				current_best_f = neighbour.second;
				
				#if defined (LOCAL_SEARCH_VERBOSE)
				std::cout
					<< setw(8)  << " "
					<< setw(15) << " "
					<< setw(18) << timing::elapsed_seconds(bbegin, timing::now())
					<< setw(18) << current_best_f
					<< setw(12) << ITERATION
					<< setw(25) << best->get_n_neighbours_explored()
					<< std::endl;
				#endif
				
				best->copy(neighbour.first);
			}
			else {
				improvement = false;
			}
			
			delete neighbour.first;
		}
		else {
			improvement = false;
		}
		
		++ITERATION;
	}
	bend = timing::now();
	total_time = timing::elapsed_seconds(bbegin, bend);
	
	return true;
}

template<class G>
void local_search<G>::print_performance() const {
	std::cout << "Local Search algorithm performance:" << std::endl;
	std::cout << "    Number of iterations:                   " << ITERATION << std::endl;
	std::cout << "    Total execution time:                   " << total_time << " s" << std::endl;
	std::cout << "    Average iteration time:                 " << total_time/ITERATION << " s" << std::endl;
	std::cout << "    Total neihgbourhood exploration time:   " << neighbourhood_time << " s" << std::endl;
	std::cout << "    Average neihgbourhood exploration time: " << neighbourhood_time/ITERATION << " s" << std::endl;
	std::cout << std::endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

