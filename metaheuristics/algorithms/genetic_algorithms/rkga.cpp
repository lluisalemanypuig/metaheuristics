#include <metaheuristics/algorithms/genetic_algorithms/rkga.hpp>

namespace metaheuristics {
namespace algorithms {

// PROTECTED

// Algorithm-related functions

template<class G, typename dT, typename cT>
void rkga<G,dT,cT>::get_two_parents(size_t& p1, size_t& p2) {
	p1 = GA<G,dT,cT>::population_rng->get_uniform();
	
	p2 = GA<G,dT,cT>::population_rng->get_uniform();
	while (p2 == p1) {
		p2 = GA<G,dT,cT>::population_rng->get_uniform();
	}
}

// Sanity check

template<class G, typename dT, typename cT>
bool rkga<G,dT,cT>::are_set_sizes_correct() const {
	// there must be at least one crossover individual
	return rkga<G,dT,cT>::N_MUTANT < rkga<G,dT,cT>::pop_size;
}

// PUBLIC

template<class G, typename dT, typename cT>
rkga<G,dT,cT>::rkga() : genetic_algorithm<G,dT,cT>() { }

template<class G, typename dT, typename cT>
rkga<G,dT,cT>::rkga
(
	size_t ps, size_t mps, size_t n_gen,
	size_t c_size, double in_p
)
: genetic_algorithm<G,dT,cT>(ps, mps, n_gen, c_size, in_p)
{
}

template<class G, typename dT, typename cT>
rkga<G,dT,cT>::~rkga() { }

template<class G, typename dT, typename cT>
bool rkga<G,dT,cT>::execute_algorithm(problem<G,dT> *best, double& current_best_f) {
	if (not are_set_sizes_correct()) {
		cerr << "Sizes chosen will lead to errors:" << endl;
		cerr << "    N_MUTANT >= pop_size" << endl;
		cerr << "    " << GA<G,dT,cT>::N_MUTANT << " >= " << GA<G,dT,cT>::pop_size << endl;
		return false;
	}
	
	// initialise random number generators
	GA<G,dT,cT>::initialise_generators();
	
	GA<G,dT,cT>::reset_algorithm();
	
	time_point bbegin, bend, send, begin, end;
	
	#ifdef GENETIC_DEBUG
	cout << "RKGA - Generating initial population (" << pop_size << ")" << endl;
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
	print_current_population();
	#endif
	
	#ifdef GENETIC_VERBOSE
	cout << setw(4)  << " "
		 << setw(18) << "Elaps. Time (s)"
	     << setw(18) << "Obj. Function"
	     << setw(12) << "Gen./" << N_GEN << endl;
	
	double best_fit = get_best_individual().get_fitness();
	double prev_best_fit = best_fit;
	
	cout << setw(4)  << "**"
		 << setw(18) << 0.0
		 << setw(18) << best_fit
		 << setw(12) << -1 << endl;
	#endif
	
	population_set next_gen(GA<G,dT,cT>::pop_size, GA<G,dT,cT>::chrom_size);
	
	bbegin = now();
	for (size_t g = 1; g <= GA<G,dT,cT>::N_GEN; ++g) {
		#ifdef GENETIC_DEBUG
		cout << "RKGA - Generating " << G << "-th generation" << endl;
		print_current_population();
		cout << "    * Generating mutants..." << endl;
		#endif
		
		size_t m = 0;
		begin = now();
		GA<G,dT,cT>::generate_mutants(best, 0, GA<G,dT,cT>::N_MUTANT, next_gen, m);
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
		send = now();
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		#endif
		
		#ifdef GENETIC_VERBOSE
		double best_fit = get_best_individual().get_fitness();
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
void rkga<G,dT,cT>::print_performance() const {
	cout << "RKGA algorithm performance:" << endl;
	cout << "    Total generation average:          " << GA<G,dT,cT>::total_time << endl;
	cout << "    Average generation average:        " << GA<G,dT,cT>::total_time/GA<G,dT,cT>::N_GEN << endl;
	cout << "    Total mutant generation time:      " << GA<G,dT,cT>::mutant_time << endl;
	cout << "    Average mutant generation time:    " << GA<G,dT,cT>::mutant_time/GA<G,dT,cT>::N_GEN << endl;
	cout << "    Total crossover generation time:   " << GA<G,dT,cT>::crossover_time << endl;
	cout << "    Average crossover generation time: " << GA<G,dT,cT>::crossover_time/GA<G,dT,cT>::N_GEN << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

