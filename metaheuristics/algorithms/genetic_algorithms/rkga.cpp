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

#include <metaheuristics/algorithms/genetic_algorithms/rkga.hpp>

namespace metaheuristics {
namespace algorithms {

// PROTECTED

// Algorithm-related functions

template<class G>
void rkga<G>::get_two_parents(size_t& p1, size_t& p2) {
	p1 = GA<G>::population_rng.get_uniform();
	
	p2 = GA<G>::population_rng.get_uniform();
	while (p2 == p1) {
		p2 = GA<G>::population_rng.get_uniform();
	}
}

// Sanity check

template<class G>
bool rkga<G>::are_set_sizes_correct() const {
	// there must be at least one crossover individual
	return rkga<G>::N_MUTANT < rkga<G>::pop_size;
}

// PUBLIC

template<class G>
rkga<G>::rkga() : genetic_algorithm<G>()
{
	
}

template<class G>
rkga<G>::rkga
(
	size_t ps, size_t mps, size_t n_gen,
	size_t c_size, double in_p
)
: genetic_algorithm<G>(ps, mps, n_gen, c_size, in_p)
{
	
}

template<class G>
rkga<G>::~rkga() { }

// MODIFIERS

template<class G>
void rkga<G>::reset_algorithm() {
	GA<G>::reset_genetic_algorithm();
}

template<class G>
bool rkga<G>::execute_algorithm(problem<G> *best, double& current_best_f) {
	if (not are_set_sizes_correct()) {
		cerr << "Sizes chosen will lead to errors:" << endl;
		cerr << "    N_MUTANT >= pop_size" << endl;
		cerr << "    " << GA<G>::N_MUTANT << " >= " << GA<G>::pop_size << endl;
		return false;
	}
	
	// initialise random number generators
	GA<G>::initialise_generators();
	// set algorithm to its initial state
	reset_algorithm();
	
	// verbose variables
	double best_fit;
	double prev_best_fit;
	
	// timing variables
	time_point bbegin, bend, send, begin, end;
	
	#if defined (GENETICS_DEBUG)
		cout << "RKGA - Generating initial population (" << GA<G>::pop_size << ")" << endl;
	#endif
	
	begin = now();
	GA<G>::initialize_population(best);
	end = now();
	GA<G>::initial_time += elapsed_seconds(begin, end);
	
	#if defined (GENETICS_DEBUG)
		GA<G>::print_current_population();
	#endif
	
	#if defined (GENETICS_VERBOSE)
		cout << setw(4)  << " "
			 << setw(18) << "Elaps. Time (s)"
			 << setw(18) << "Obj. Function"
			 << setw(12) << "Gen./" << GA<G>::N_GEN << endl;
		
		best_fit = GA<G>::get_best_individual().get_fitness();
		prev_best_fit = best_fit;
		
		cout << setw(4)  << "**"
			 << setw(18) << GA<G>::initial_time
			 << setw(18) << best_fit
			 << setw(12) << -1 << endl;
	#endif
	
	population_set next_gen(GA<G>::pop_size, GA<G>::chrom_size);
	
	bbegin = now();
	for (size_t g = 1; g <= GA<G>::N_GEN; ++g) {
		#if defined (GENETICS_DEBUG)
			cout << "RKGA - Generating " << g << "-th generation" << endl;
			GA<G>::print_current_population();
			cout << "    * Generating mutants..." << endl;
		#endif
		
		size_t m = 0;
		begin = now();
		GA<G>::generate_mutants(best, 0, GA<G>::N_MUTANT, next_gen, m);
		end = now();
		GA<G>::mutant_time += elapsed_seconds(begin, end);
		
		#if defined (GENETICS_DEBUG)
			cout << endl;
			cout << "    * Generating crossover..." << endl;
		#endif
		
		begin = now();
		GA<G>::generate_crossovers(best, next_gen, m);
		end = now();
		GA<G>::crossover_time += elapsed_seconds(begin, end);
		
		#if defined (GENETICS_DEBUG)
			cout << endl;
			cout << "    * Swapping generations..." << endl;
		#endif
		
		GA<G>::population = next_gen;
		send = now();
		
		#if defined (GENETICS_DEBUG)
			cout << endl;
		#endif
		
		#if defined (GENETICS_VERBOSE)
			best_fit = GA<G>::get_best_individual().get_fitness();
			double etime = elapsed_seconds(bbegin, send);
			
			if (best_fit > prev_best_fit) {
				prev_best_fit = best_fit;
				cout << setw(4)  << "**";
			}
			else {
				cout << setw(4)  << "  ";
			}
			
			cout << setw(18) << etime
				 << setw(18) << best_fit
				 << setw(12) << g << endl;
		#endif
	}
	bend = now();
	GA<G>::total_time += elapsed_seconds(bbegin, bend);
	
	#if defined (GENETICS_VERBOSE)
		cout << "Generated all generations in " << GA<G>::total_time << " s" << endl;
	#endif
	
	const individual& fittest_individual = GA<G>::get_best_individual();
	current_best_f = best->decode(fittest_individual.get_chromosome());
	
	return true;
}

template<class G>
void rkga<G>::print_performance() const {
	cout << "RKGA algorithm performance:" << endl;
	cout << "    Total generation average:          " << GA<G>::total_time << endl;
	cout << "    Average generation average:        " << GA<G>::total_time/GA<G>::N_GEN << endl;
	cout << "    Total mutant generation time:      " << GA<G>::mutant_time << endl;
	cout << "    Average mutant generation time:    " << GA<G>::mutant_time/GA<G>::N_GEN << endl;
	cout << "    Total crossover generation time:   " << GA<G>::crossover_time << endl;
	cout << "    Average crossover generation time: " << GA<G>::crossover_time/GA<G>::N_GEN << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

