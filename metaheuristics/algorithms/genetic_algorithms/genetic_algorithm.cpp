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

#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.hpp>

namespace metaheuristics {
namespace algorithms {

// PROTECTED

// Information display functions

template<class G>
void genetic_algorithm<G>::print_current_population() const {
	cout << "    * Current population:" << endl;
	for (size_t i = 0; i < pop_size; ++i) {
		cout << "        " << i << ": " << population[i] << endl;
	}
	cout << endl;
}

// Algorithm-related functions

template<class G>
void genetic_algorithm<G>::initialize_population(const problem<G> *p) {
	population.clear();
	population.resize(pop_size, chrom_size);
	
	for (size_t i = 0; i < pop_size; ++i) {
		#if defined (GENETICS_DEBUG)
			cout << "    pre: " << i << ": " << population[i] << endl;
		#endif
		
		generate_mutant(p, population[i]);
		
		#if defined (GENETICS_DEBUG)
			cout << "    post: " << i << ": " << population[i] << endl;
			cout << endl;
		#endif
	}
}

template<class G>
void genetic_algorithm<G>::generate_mutants(const problem<G> *p, size_t A, size_t B, population_set& next_gen, size_t& m) {
	for (m = A; m < B; ++m) {
		generate_mutant(p, next_gen[m]);
		
		#if defined (GENETICS_DEBUG)
			cout << "        " << m << ": " << next_gen[m] << endl;
		#endif
	}
}

template<class G>
void genetic_algorithm<G>::generate_crossovers(const problem<G> *p, population_set& next_gen, size_t& m) {
	size_t par1_idx, par2_idx;
	
	for (; m < pop_size; ++m) {	
		get_two_parents(par1_idx, par2_idx);
		crossover(p, par1_idx, par2_idx, next_gen[m]);
		
		#if defined (GENETICS_DEBUG)
			cout << "        "
				 << m << ": (" << par1_idx << " x " << par2_idx << ")"
				 << "    " << next_gen[m] << endl;
		#endif
	}
}

// Population-generation functions

template<class G>
void genetic_algorithm<G>::evaluate_individual(const problem<G> *p, individual& i) const {
	problem<G> *copy = p->clone();
	
	try {
		// decode the chromosome into a solution of the problem<G>
		double F = copy->decode(i.get_chromosome());
		
		#if defined (GENETICS_DEBUG)
			if (not copy->sanity_check()) {
				cerr << "void genetic_algorithm<G>::evaluate_individual:" << endl;
				cerr << "    Decoded solution from chromosome is not sane" << endl;
			}
		#endif
		
		// store the quality of the solution in the individual
		i.set_fitness(F);
		
		// free memory
		delete copy;
	}
	catch (const infeasible_exception& e) {
		// store the quality of the solution in the individual
		i.set_fitness(-numeric_limits<double>::max());

		// free memory
		delete copy;
	}
}

template<class G>
void genetic_algorithm<G>::generate_mutant(const problem<G> *p, individual& i) {
	// generate chromosome
	zero_one_rng.make_n_uniform(&i.get_chromosome()[0], i.get_chromosome().size());
	
	// compute the fitness of an individual
	evaluate_individual(p, i);
}

template<class G>
void genetic_algorithm<G>::crossover(const problem<G> *p, size_t par1_idx, size_t par2_idx, individual& son) {
	const individual& parent1 = population[par1_idx];
	const individual& parent2 = population[par2_idx];
	
	for (size_t i = 0; i < chrom_size; ++i) {
		double g1 = parent1.get_gene(i);
		double g2 = parent2.get_gene(i);
		
		double rand = zero_one_rng.get_uniform();
		double selected_gene = (rand <= in_prob ? g1 : g2);
		
		son.set_gene(i, selected_gene);
	}
	
	evaluate_individual(p, son);
}

template<class G>
void genetic_algorithm<G>::reset_genetic_algorithm() {
	total_time = 0.0;
	initial_time = 0.0;
	crossover_time = 0.0;
	mutant_time = 0.0;
}

template<class G>
void genetic_algorithm<G>::initialise_generators() {
	zero_one_rng.init_uniform(0, 1);
	population_rng.init_uniform(0, pop_size - 1);
	
	if (META<G>::seed_rng) {
		zero_one_rng.seed_random_engine();
		population_rng.seed_random_engine();
	}
}

// PUBLIC

template<class G>
genetic_algorithm<G>::genetic_algorithm() : metaheuristic<G>() {
	pop_size = 0;
	N_MUTANT = 0;
	N_GEN = 0;
	chrom_size = 0;
	in_prob = 0;
	
	reset_genetic_algorithm();
}

template<class G>
genetic_algorithm<G>::genetic_algorithm
(
	size_t ps, size_t mps, size_t n_gen,
	size_t c_size, double in_p
)
: metaheuristic<G>()
{
	pop_size = ps;
	N_MUTANT = mps;
	N_GEN = n_gen;
	chrom_size = c_size;
	in_prob = in_p;
	
	reset_genetic_algorithm();
}

template<class G>
genetic_algorithm<G>::~genetic_algorithm() { }

template<class G>
const individual& genetic_algorithm<G>::get_best_individual() const {
	double f = -numeric_limits<double>::max();
	size_t best = 0;
	
	for (size_t i = 0; i < pop_size; ++i) {
		if (f < population[i].get_fitness()) {
			f = population[i].get_fitness();
			best = i;
		}
	}
	
	return population[best];
}

template<class G>
const individual& genetic_algorithm<G>::get_individual(size_t i) const {
	assert(i < pop_size);
	return population[i];
}

template<class G>
size_t genetic_algorithm<G>::population_size() const {
	return population.size();
}

} // -- namespace algorithms
} // -- namespace metaheuristics

