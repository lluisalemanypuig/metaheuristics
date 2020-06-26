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

#include <metaheuristics/algorithms/genetic_algorithms/brkga.hpp>

// C++ includes
#include <queue>

#if defined (GENETICS_VERBOSE)
#include <iostream>
#include <iomanip>
#endif

// metaheursitics includes
#include <metaheuristics/misc/time.hpp>
#include <metaheuristics/macros.hpp>

namespace metaheuristics {
namespace algorithms {

// PUBLIC

template<class G>
brkga<G>::brkga() : genetic_algorithm<G>() {
	N_ELITE = 0;
}

template<class G>
brkga<G>::brkga
(
	size_t ps, size_t mps, size_t ess, size_t n_gen,
	size_t c_size, double in_p
)
: genetic_algorithm<G>(ps, mps, n_gen, c_size, in_p)
{
	N_ELITE = ess;
}

template<class G>
brkga<G>::~brkga() { }

template<class G>
void brkga<G>::reset_algorithm() {
	GA<G>::reset_genetic_algorithm();

	elite_copying_time = 0.0;
}

template<class G>
const structures::individual& brkga<G>::get_best_individual() const {
	return GA<G>::population[ elite_set[0].second ];
}

template<class G>
bool brkga<G>::execute_algorithm(structures::problem<G> *best, double& current_best_f) {
	if (not are_set_sizes_correct()) {
		std::cerr << MH_ERROR << std::endl;
		std::cerr << "    Sizes chosen will lead to errors:" << std::endl;
		std::cerr << "    NUMBER OF MUTANTS + NUMBER OF ELITE >= POPULATION SIZE" << std::endl;
		std::cerr << "    " << GA<G>::N_MUTANT + N_ELITE << " >= "
				  << GA<G>::pop_size << std::endl;
		return false;
	}
	
	// initialise random number generators
	GA<G>::initialise_generators();
	elite_rng.init_uniform(0, N_ELITE - 1);
	if (META<G>::seed_rng) {
		elite_rng.seed_random_engine();
	}
	// set algorithm to its initial state
	reset_algorithm();
	
	#if defined (GENETICS_DEBUG)
	// verbose variables
	double prev_best_fit;
	double best_fit;
	#endif
	
	// timing variables
	timing::time_point bbegin, bend, send, begin, end;
	
	#if defined (GENETICS_DEBUG)
	std::cout << "BRKGA - Generating initial population (" << GA<G>::pop_size << ")" << std::endl;
	#endif
	
	begin = timing::now();
	GA<G>::initialize_population(best);
	end = timing::now();
	GA<G>::initial_time += timing::elapsed_seconds(begin, end);
	
	#if defined (GENETICS_DEBUG)
	std::cout << "BRKGA - Initializing elite set (" << N_ELITE << ")" << std::endl;
	#endif
	
	elite_set.resize(N_ELITE);
	track_elite_individuals();
	
	#if defined (GENETICS_DEBUG)
	GA<G>::print_current_population();
	print_elite_set();
	#endif
	
	#if defined (GENETICS_VERBOSE)
	std::cout
		<< setw(8)  << " "
		<< setw(18) << "Elaps. Time (s)"
		<< setw(18) << "Obj. Function"
		<< setw(12) << "Gen./" << GA<G>::N_GEN
		<< std::endl;

	best_fit = GA<G>::get_best_individual().get_fitness();
	prev_best_fit = best_fit;

	std::cout
		<< setw(8)  << "**"
		<< setw(18) << GA<G>::initial_time
		<< setw(18) << best_fit
		<< setw(12) << 0
		<< std::endl;
	#endif
	
	population_set next_gen(GA<G>::pop_size, GA<G>::chrom_size);
	
	bbegin = timing::now();
	for (size_t g = 1; g <= GA<G>::N_GEN; ++g) {
		
		#if defined (GENETICS_DEBUG)
		std::cout << "BRKGA - Generating " << g << "-th generation" << std::endl;
		GA<G>::print_current_population();
		print_elite_set();
		std::cout << "    * Copying elite individuals..." << std::endl;
		#endif
		
		size_t m = 0;
		
		begin = timing::now();
		copy_elite_individuals(best, next_gen, m);
		end = timing::now();
		elite_copying_time += timing::elapsed_seconds(begin, end);
		
		#if defined (GENETICS_DEBUG)
		std::cout << std::endl;
		std::cout << "    * Generating mutants..." << std::endl;
		#endif
		
		begin = timing::now();
		GA<G>::generate_mutants(best, m, N_ELITE + GA<G>::N_MUTANT, next_gen, m);
		end = timing::now();
		GA<G>::mutant_time += timing::elapsed_seconds(begin, end);
		
		#if defined (GENETICS_DEBUG)
		std::cout << std::endl;
		std::cout << "    * Generating crossover..." << std::endl;
		#endif
		
		begin = timing::now();
		GA<G>::generate_crossovers(best, next_gen, m);
		end = timing::now();
		GA<G>::crossover_time += timing::elapsed_seconds(begin, end);
		
		#if defined (GENETICS_DEBUG)
		std::cout << std::endl;
		std::cout << "    * Swapping generations..." << std::endl;
		#endif
		
		GA<G>::population = next_gen;
		track_elite_individuals();
		
		send = timing::now();
		
		#if defined (GENETICS_DEBUG)
		std::cout << std::endl;
		#endif
		
		#if defined (GENETICS_VERBOSE)
		best_fit = GA<G>::get_best_individual().get_fitness();
		double etime = timing::elapsed_seconds(bbegin, send) + GA<G>::initial_time;

		if (best_fit > prev_best_fit) {
			prev_best_fit = best_fit;
			std::cout << setw(8)  << "**";
		}
		else {
			std::cout << setw(8)  << " ";
		}

		std::cout
			<< setw(18) << etime
			<< setw(18) << best_fit
			<< setw(12) << g
			<< std::endl;

		#endif
	}
	bend = timing::now();
	GA<G>::total_time += timing::elapsed_seconds(bbegin, bend);
	
	#if defined (GENETICS_VERBOSE)
	std::cout << "Generated all generations in " << GA<G>::total_time << " s" << std::endl;
	#endif
	
	const structures::individual& fittest_individual = GA<G>::get_best_individual();
	current_best_f = best->decode(fittest_individual.get_chromosome());
	
	return true;
}

template<class G>
void brkga<G>::print_performance() const {
	std::cout << "BRKGA algorithm performance:" << std::endl;
	std::cout << "    Total generation time:             " << GA<G>::total_time << " s" << std::endl;
	std::cout << "    Average generation average:        " << GA<G>::total_time/static_cast<double>(GA<G>::N_GEN) << " s" << std::endl;
	std::cout << "    Total copying elite time:          " << elite_copying_time << " s" << std::endl;
	std::cout << "    Average copying elite time:        " << elite_copying_time/static_cast<double>(GA<G>::N_GEN) << " s" << std::endl;
	std::cout << "    Total mutant generation time:      " << GA<G>::mutant_time << " s" << std::endl;
	std::cout << "    Average mutant generation time:    " << GA<G>::mutant_time/static_cast<double>(GA<G>::N_GEN) << " s" << std::endl;
	std::cout << "    Total crossover generation time:   " << GA<G>::crossover_time << " s" << std::endl;
	std::cout << "    Average crossover generation time: " << GA<G>::crossover_time/static_cast<double>(GA<G>::N_GEN) << " s" << std::endl;
	std::cout << std::endl;
}

// Information display functions

template<class G>
void brkga<G>::print_elite_set() const {
	for (size_t it = 0; it < N_ELITE; ++it) {
		std::cout << "        elite idx= " << elite_set[it].second << std::endl;
	}
	std::cout << std::endl;
}

// Population-generation functions

template<class G>
void brkga<G>::get_two_parents(size_t& p1, size_t& p2) {
	size_t idx1 = elite_rng.get_uniform();
	p1 = elite_set[idx1].second;

	p2 = GA<G>::population_rng.get_uniform();
	while (p2 == p1 or is_elite_individual(p2)) {
		p2 = GA<G>::population_rng.get_uniform();
	}
}

template<class G>
void brkga<G>::copy_elite_individuals(
	const structures::problem<G> *,
	population_set& next_gen, size_t& m
)
{
	assert(m < (GA<G>::pop_size) );

	for (size_t it = 0; it < N_ELITE; ++it) {
		next_gen[m + it] = GA<G>::population[ elite_set[it].second ];
	}
	m += N_ELITE;
}

template<class G>
bool brkga<G>::is_elite_individual(size_t idx) const {
	assert(idx < (GA<G>::pop_size) );

	size_t it = 0;
	while (it < N_ELITE) {
		if (elite_set[it].second == idx) {
			return true;
		}
		++it;
	}

	return false;
}

template<class G>
void brkga<G>::track_elite_individuals() {
	// sort individuals by their fitness
	std::priority_queue<std::pair<double, size_t> > fitness_individual;
	for (size_t i = 0; i < GA<G>::pop_size; ++i) {
		fitness_individual.push(
			std::pair<double, size_t>(
				GA<G>::population[i].get_fitness(),
				i
			)
		);
	}

	// fill in elite_set
	for (size_t eidx = 0; eidx < N_ELITE; ++eidx) {
		elite_set[eidx] = fitness_individual.top();
		fitness_individual.pop();
	}
}

// Sanity check

template<class G>
bool brkga<G>::are_set_sizes_correct() const {
	// there must be at least one crossover individual
	return N_ELITE + GA<G>::N_MUTANT < GA<G>::pop_size;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

