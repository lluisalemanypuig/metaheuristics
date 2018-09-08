#include <metaheuristics/algorithms/genetic_algorithms/brkga.hpp>

namespace metaheuristics {
namespace algorithms {

// Information display functions

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::print_elite_set() const {
	for (size_t it = 0; it < N_ELITE; ++it) {
		cout << "        elite idx= " << elite_set[it].second << endl;
	}
	cout << endl;
}

// Population-generation functions

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::get_two_parents(size_t& p1, size_t& p2) {
	size_t idx1 = rng->get_next_rand_int(0, N_ELITE - 1);
	p1 = elite_set[idx1].second;
	
	p2 = rng->get_next_rand_int(0, pop_size - 1);
	while (p2 == p1 or is_elite_individual(p2)) {
		p2 = rng->get_next_rand_int(0, pop_size - 1);
	}
}

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::copy_elite_individuals(const problem *p, population_set& next_gen, size_t& m) {
	assert(m < pop_size);
	
	for (size_t it = 0; it < N_ELITE; ++it) {
		next_gen[m + it] = population[ elite_set[it].second ];
	}
	m += N_ELITE;
}

template<class G, typename dT, typename cT>
bool brkga<G,dT,cT>::is_elite_individual(size_t idx) const {
	assert(idx < pop_size);
	
	bool is_elite = false;
	size_t it = 0;
	while (it < N_ELITE and not is_elite) {
		if (elite_set[it].second == idx) {
			is_elite = true;
		}
		++it;
	}
}

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::track_elite_individuals() {
	// sort individuals by their fitness
	priority_queue<pair<double, size_t> > fitness_individual;
	for (size_t i = 0; i < pop_size; ++i) {
		fitness_individual.push(pair<double, size_t>(population[i].get_fitness(), i));
	}
	
	// fill in elite_set
	for (size_t eidx = 0; eidx < N_ELITE; ++eidx) {
		elite_set[eidx] = fitness_individual.top();
		fitness_individual.pop();
	}
}

// Sanity check

template<class G, typename dT, typename cT>
bool brkga<G,dT,cT>::are_set_sizes_correct() const {
	// there must be at least one crossover individual
	return N_ELITE + N_MUTANT < pop_size;
}

// PUBLIC

template<class G, typename dT, typename cT>
brkga<G,dT,cT>::brkga() : genetic_algorithm<G,dT,cT>() {
	N_ELITE = 0;
	
	reset_algorithm();
}

template<class G, typename dT, typename cT>
brkga<G,dT,cT>::brkga
(
	size_t ps, size_t mps, size_t ess, size_t n_gen,
	size_t c_size, double in_p,
	drandom_generator<G,dT> *drng,
	crandom_generator<G,cT> *crng
)
: genetic_algorithm<G,dT,cT>(ps, mps, n_gen, c_size, in_p, drng, crng)
{
	N_ELITE = ess;
	
	reset_algorithm();
}

template<class G, typename dT, typename cT>
brkga<G,dT,cT>::~brkga() { }

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::reset_algorithm() {
	elite_copying_time = 0.0;
	
	reset_genetic_algorithm();
}

template<class G, typename dT, typename cT>
const individual& brkga<G,dT,cT>::get_best_individual() const {
	return population[ elite_set[0].second ];
}

template<class G, typename dT, typename cT>
bool brkga<G,dT,cT>::execute_algorithm(problem *best, double& current_best_f) {
	if (not are_set_sizes_correct()) {
		cerr << "brkga<G,dT,cT>::execute_algorithm:" << endl;
		cerr << "    Sizes chosen will lead to errors:" << endl;
		cerr << "        NUMBER OF MUTANTS + NUMBER OF ELITE >= POPULATION SIZE" << endl;
		cerr << "        " << N_MUTANT + N_ELITE << " >= " << pop_size << endl;
		return false;
	}
	
	if (rng == NULL) {
		cerr << "BRKGA : Error" << endl;
		cerr << "bool brkga<G,dT,cT>::execute_algorithm - RNG is not set" << endl;
		return false;
	}
	
	double bbegin, bend, send, begin, end;
	
	#ifdef GENETIC_DEBUG
	cout << "BRKGA - Generating initial population (" << pop_size << ")" << endl;
	#endif
	
	begin = now();
	population = population_set(pop_size, chrom_size);
	initialize_population(best);
	end = now();
	initial_time += elapsed_time(begin, end);
	
	#ifdef GENETIC_DEBUG
	cout << "BRKGA - Initializing elite set (" << N_ELITE << ")" << endl;
	#endif
	
	elite_set.resize(N_ELITE);
	track_elite_individuals();
	
	#ifdef GENETIC_DEBUG
	print_current_population();
	print_elite_set();
	#endif
	
	#ifdef GENETIC_VERBOSE
	cout << setw(8)  << " "
		 << setw(8)  << " "
		 << setw(18) << "Elaps. Time (s)"
	     << setw(18) << "Obj. Function"
	     << setw(12) << "Gen./" << N_GEN << endl;
	
	double best_fit = get_best_individual().get_fitness();
	double prev_best_fit = best_fit;
	
	cout << setw(8)  << " "
		 << setw(8)  << "**"
		 << setw(18) << initial_time
		 << setw(18) << best_fit
		 << setw(12) << 0 << endl;
	#endif
	
	population_set next_gen(pop_size, chrom_size);
	
	bbegin = now();
	for (size_t G = 1; G <= N_GEN; ++G) {
		
		#ifdef GENETIC_DEBUG
		cout << "BRKGA - Generating " << G << "-th generation" << endl;
		print_current_population();
		print_elite_set();
		cout << "    * Copying elite individuals..." << endl;
		#endif
		
		size_t m = 0;
		
		begin = now();
		copy_elite_individuals(best, next_gen, m);
		end = now();
		elite_copying_time += elapsed_time(begin, end);
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		cout << "    * Generating mutants..." << endl;
		#endif
		
		begin = now();
		generate_mutants(best, m, N_ELITE + N_MUTANT, next_gen, m);
		end = now();
		mutant_time += elapsed_time(begin, end);
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		cout << "    * Generating crossover..." << endl;
		#endif
		
		begin = now();
		generate_crossovers(best, next_gen, m);
		end = now();
		crossover_time += elapsed_time(begin, end);
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		cout << "    * Swapping generations..." << endl;
		#endif
		
		population = next_gen;
		track_elite_individuals();
		
		send = now();
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		#endif
		
		#ifdef GENETIC_VERBOSE
		double best_fit = get_best_individual().get_fitness();
		double etime = elapsed_time(bbegin, send) + initial_time;
		
		if (best_fit > prev_best_fit) {
			prev_best_fit = best_fit;
			cout << setw(8)  << " "
				 << setw(8)  << "**";
		}
		else {
			cout << setw(8)  << " "
				 << setw(8)  << " ";
		}
		
		cout << setw(18) << etime
			 << setw(18) << best_fit
			 << setw(12) << G << endl;
		#endif
	}
	bend = now();
	total_time += elapsed_time(bbegin, bend);
	
	#ifdef GENETIC_DEBUG
	cout << "Generated all generations in " << total_time << " s" << endl;
	#endif
	
	const individual& fittest_individual = get_best_individual();
	current_best_f = best->decode(fittest_individual.get_chromosome());
	
	return true;
}

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::print_performance() const {
	cout << "BRKGA algorithm performance:" << endl;
	cout << "    Total generation time:             " << total_time << " s" << endl;
	cout << "    Average generation average:        " << total_time/N_GEN << " s" << endl;
	cout << "    Total copying elite time:          " << elite_copying_time << " s" << endl;
	cout << "    Average copying elite time:        " << elite_copying_time/N_GEN << " s" << endl;
	cout << "    Total mutant generation time:      " << mutant_time << " s" << endl;
	cout << "    Average mutant generation time:    " << mutant_time/N_GEN << " s" << endl;
	cout << "    Total crossover generation time:   " << crossover_time << " s" << endl;
	cout << "    Average crossover generation time: " << crossover_time/N_GEN << " s" << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

