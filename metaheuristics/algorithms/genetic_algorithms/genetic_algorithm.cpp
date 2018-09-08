#include <metaheuristics/algorithms/genetic_algorithms/genetic_algorithm.hpp>

namespace metaheuristics {
namespace algorithms {

// PROTECTED

// Information display functions

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::print_current_population() const {
	cout << "    * Current population:" << endl;
	for (size_t i = 0; i < pop_size; ++i) {
		cout << "        " << i << ": " << population[i] << endl;
	}
	cout << endl;
}

// Algorithm-related functions

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::initialize_population(const problem<G,dT> *p) {
	for (size_t i = 0; i < pop_size; ++i) {
		
		#if GENETIC_DEBUG
		cout << "    pre: " << i << ": " << population[i] << endl;
		#endif
		
		generate_mutant(p, population[i]);
		
		#if GENETIC_DEBUG
		cout << "    post: " << i << ": " << population[i] << endl;
		cout << endl;
		#endif
	}
}

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::generate_mutants(const problem<G,dT> *p, size_t A, size_t B, population_set& next_gen, size_t& m) {
	for (m = A; m < B; ++m) {
		generate_mutant(p, next_gen[m]);
		
		#if GENETIC_DEBUG
		cout << "        " << m << ": " << next_gen[m] << endl;
		#endif
	}
}

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::generate_crossovers(const problem<G,dT> *p, population_set& next_gen, size_t& m) {
	size_t par1_idx, par2_idx;
	
	for (; m < pop_size; ++m) {	
		get_two_parents(par1_idx, par2_idx);
		crossover(p, par1_idx, par2_idx, next_gen[m]);
		
		#if GENETIC_DEBUG
		cout << "        " << m << ": (" << par1_idx << " x " << par2_idx << ") " << next_gen[m] << endl;
		#endif
	}
}

// Population-generation functions

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::evaluate_individual(const problem<G,dT> *p, individual& i) const {
	// decode chromosome into a solution
	problem<G,dT> *copy = p->clone();
	
	try {
		// decode the chromosome into a solution of the problem<G,dT>
		double F = copy->decode(i.get_chromosome());
		
		#ifdef GENETIC_DEBUG
		bool sane = copy->sanity_check();
		if (not sane) {
			cerr << "void genetic_algorithm<G,dT,cT>::evaluate_individual:" << endl;
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

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::generate_mutant(const problem<G,dT> *p, individual& i) {
	// generate chromosome
	rng->get_n_rand(&i.get_chromosome()[0], i.get_chromosome().size());
	
	// compute the fitness of an individual
	evaluate_individual(p, i);
}

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::crossover(const problem<G,dT> *p, size_t par1_idx, size_t par2_idx, individual& son) {
	const individual& parent1 = population[par1_idx];
	const individual& parent2 = population[par2_idx];
	
	for (size_t i = 0; i < chrom_size; ++i) {
		double g1 = parent1.get_gene(i);
		double g2 = parent2.get_gene(i);
		
		double rand = rng->get_next_rand();
		double selected_gene = (rand <= in_prob ? g1 : g2);
		
		son.set_gene(i, selected_gene);
	}
	
	evaluate_individual(p, son);
}

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::reset_genetic_algorithm() {
	total_time = 0.0;
	initial_time = 0.0;
	crossover_time = 0.0;
	mutant_time = 0.0;
}

// PUBLIC

template<class G, typename dT, typename cT>
genetic_algorithm<G,dT,cT>::genetic_algorithm() : metaheuristic<G,dT,cT>() {
	pop_size = 0;
	N_MUTANT = 0;
	N_GEN = 0;
	chrom_size = 0;
	in_prob = 0;
	
	reset_genetic_algorithm();
}

template<class G, typename dT, typename cT>
genetic_algorithm<G,dT,cT>::genetic_algorithm
(
	size_t ps, size_t mps, size_t n_gen,
	size_t c_size, double in_p,
	drandom_generator<G,dT> *drng,
	crandom_generator<G,cT> *crng
)
: metaheuristic<G,dT,cT>(drng,crng)
{
	pop_size = ps;
	N_MUTANT = mps;
	N_GEN = n_gen;
	chrom_size = c_size;
	in_prob = in_p;
	
	reset_algorithm();
}

template<class G, typename dT, typename cT>
genetic_algorithm<G,dT,cT>::~genetic_algorithm() { }

template<class G, typename dT, typename cT>
void genetic_algorithm<G,dT,cT>::reset_algorithm() {
	reset_genetic_algorithm();
}

template<class G, typename dT, typename cT>
const individual& genetic_algorithm<G,dT,cT>::get_best_individual() const {
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

template<class G, typename dT, typename cT>
const individual& genetic_algorithm<G,dT,cT>::get_individual(size_t i) const {
	assert(i < pop_size);
	return population[i];
}

template<class G, typename dT, typename cT>
size_t genetic_algorithm<G,dT,cT>::population_size() const {
	return population.size();
}

} // -- namespace algorithms
} // -- namespace metaheuristics

