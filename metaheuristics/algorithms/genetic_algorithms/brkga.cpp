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
	size_t idx1 = elite_rng.get_uniform();
	p1 = elite_set[idx1].second;
	
	p2 = GA<G,dT,cT>::population_rng.get_uniform();
	while (p2 == p1 or is_elite_individual(p2)) {
		p2 = GA<G,dT,cT>::population_rng.get_uniform();
	}
}

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::copy_elite_individuals(const problem<G,dT> *p, population_set& next_gen, size_t& m) {
	assert(m < (GA<G,dT,cT>::pop_size) );
	
	for (size_t it = 0; it < N_ELITE; ++it) {
		next_gen[m + it] = GA<G,dT,cT>::population[ elite_set[it].second ];
	}
	m += N_ELITE;
}

template<class G, typename dT, typename cT>
bool brkga<G,dT,cT>::is_elite_individual(size_t idx) const {
	assert(idx < (GA<G,dT,cT>::pop_size) );
	
	size_t it = 0;
	while (it < N_ELITE) {
		if (elite_set[it].second == idx) {
			return true;
		}
		++it;
	}
	
	return false;
}

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::track_elite_individuals() {
	// sort individuals by their fitness
	priority_queue<pair<double, size_t> > fitness_individual;
	for (size_t i = 0; i < GA<G,dT,cT>::pop_size; ++i) {
		fitness_individual.push(
			pair<double, size_t>(
				GA<G,dT,cT>::population[i].get_fitness(),
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

template<class G, typename dT, typename cT>
bool brkga<G,dT,cT>::are_set_sizes_correct() const {
	// there must be at least one crossover individual
	return N_ELITE + GA<G,dT,cT>::N_MUTANT < GA<G,dT,cT>::pop_size;
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
	size_t c_size, double in_p
)
: genetic_algorithm<G,dT,cT>(ps, mps, n_gen, c_size, in_p)
{
	N_ELITE = ess;
	
	reset_algorithm();
}

template<class G, typename dT, typename cT>
brkga<G,dT,cT>::~brkga() { }

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::reset_algorithm() {
	elite_copying_time = 0.0;
	
	GA<G,dT,cT>::reset_genetic_algorithm();
}

template<class G, typename dT, typename cT>
const individual& brkga<G,dT,cT>::get_best_individual() const {
	return GA<G,dT,cT>::population[ elite_set[0].second ];
}

template<class G, typename dT, typename cT>
bool brkga<G,dT,cT>::execute_algorithm(problem<G,dT> *best, double& current_best_f) {
	if (not are_set_sizes_correct()) {
		cerr << "brkga<G,dT,cT>::execute_algorithm:" << endl;
		cerr << "    Sizes chosen will lead to errors:" << endl;
		cerr << "        NUMBER OF MUTANTS + NUMBER OF ELITE >= POPULATION SIZE" << endl;
		cerr << "        " << GA<G,dT,cT>::N_MUTANT + N_ELITE << " >= "
			 << GA<G,dT,cT>::pop_size << endl;
		return false;
	}
	
	// initialise random number generators
	GA<G,dT,cT>::initialise_generators();
	elite_rng.init_uniform(0, N_ELITE - 1);
	
	time_point bbegin, bend, send, begin, end;
	
	#ifdef GENETIC_DEBUG
	cout << "BRKGA - Generating initial population (" << GA<G,dT,cT>::pop_size << ")" << endl;
	#endif
	
	begin = now();
	size_t __pop_size = GA<G,dT,cT>::pop_size;
	size_t __chrom_size = GA<G,dT,cT>::chrom_size;
	GA<G,dT,cT>::population.clear();
	GA<G,dT,cT>::population.resize(__pop_size, __chrom_size);
	GA<G,dT,cT>::initialize_population(best);
	end = now();
	GA<G,dT,cT>::initial_time += elapsed_seconds(begin, end);
	
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
	
	population_set next_gen(GA<G,dT,cT>::pop_size, GA<G,dT,cT>::chrom_size);
	
	bbegin = now();
	for (size_t g = 1; g <= GA<G,dT,cT>::N_GEN; ++g) {
		
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
		elite_copying_time += elapsed_seconds(begin, end);
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		cout << "    * Generating mutants..." << endl;
		#endif
		
		begin = now();
		GA<G,dT,cT>::generate_mutants(best, m, N_ELITE + GA<G,dT,cT>::N_MUTANT, next_gen, m);
		end = now();
		GA<G,dT,cT>::mutant_time += elapsed_seconds(begin, end);
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		cout << "    * Generating crossover..." << endl;
		#endif
		
		begin = now();
		GA<G,dT,cT>::generate_crossovers(best, next_gen, m);
		end = now();
		GA<G,dT,cT>::crossover_time += elapsed_seconds(begin, end);
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		cout << "    * Swapping generations..." << endl;
		#endif
		
		GA<G,dT,cT>::population = next_gen;
		track_elite_individuals();
		
		send = now();
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		#endif
		
		#ifdef GENETIC_VERBOSE
		double best_fit = get_best_individual().get_fitness();
		double etime = elapsed_seconds(bbegin, send) + initial_time;
		
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
	GA<G,dT,cT>::total_time += elapsed_seconds(bbegin, bend);
	
	#ifdef GENETIC_DEBUG
	cout << "Generated all generations in " << total_time << " s" << endl;
	#endif
	
	const individual& fittest_individual = GA<G,dT,cT>::get_best_individual();
	current_best_f = best->decode(fittest_individual.get_chromosome());
	
	return true;
}

template<class G, typename dT, typename cT>
void brkga<G,dT,cT>::print_performance() const {
	cout << "BRKGA algorithm performance:" << endl;
	cout << "    Total generation time:             " << GA<G,dT,cT>::total_time << " s" << endl;
	cout << "    Average generation average:        " << GA<G,dT,cT>::total_time/GA<G,dT,cT>::N_GEN << " s" << endl;
	cout << "    Total copying elite time:          " << elite_copying_time << " s" << endl;
	cout << "    Average copying elite time:        " << elite_copying_time/GA<G,dT,cT>::N_GEN << " s" << endl;
	cout << "    Total mutant generation time:      " << GA<G,dT,cT>::mutant_time << " s" << endl;
	cout << "    Average mutant generation time:    " << GA<G,dT,cT>::mutant_time/GA<G,dT,cT>::N_GEN << " s" << endl;
	cout << "    Total crossover generation time:   " << GA<G,dT,cT>::crossover_time << " s" << endl;
	cout << "    Average crossover generation time: " << GA<G,dT,cT>::crossover_time/GA<G,dT,cT>::N_GEN << " s" << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

