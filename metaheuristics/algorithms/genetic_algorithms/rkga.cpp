#include <metaheuristics/algorithms/genetic_algorithms/rkga.hpp>

namespace metaheuristics {
namespace algorithms {

// PROTECTED

// Algorithm-related functions

void rkga::get_two_parents(size_t& p1, size_t& p2) {
	p1 = rng->get_next_rand_int(0, population.size() - 1);
	
	p2 = rng->get_next_rand_int(0, population.size() - 1);
	while (p2 == p1) {
		p2 = rng->get_next_rand_int(0, population.size() - 1);
	}
}

// Sanity check

bool rkga::are_set_sizes_correct() const {
	return N_MUTANT < pop_size;
}

// PUBLIC

rkga::rkga() : genetic_algorithm() { }

rkga::rkga
(
	size_t ps, size_t mps, size_t n_gen,
	size_t c_size, double in_p,
	random_number_generator *r
)
: genetic_algorithm(ps, mps, n_gen, c_size, in_p, r)
{
}

rkga::~rkga() { }

bool rkga::execute_algorithm(problem *best, double& current_best_f) {
	if (not are_set_sizes_correct()) {
		cerr << "Sizes chosen will lead to errors:" << endl;
		cerr << "    N_MUTANT >= pop_size" << endl;
		cerr << "    " << N_MUTANT << " >= " << pop_size << endl;
		return false;
	}
	
	if (rng == NULL) {
		cerr << "RKGA : Error" << endl;
		cerr << "bool rkga::execute_algorithm - RNG is not set" << endl;
		return false;
	}
	
	reset_algorithm();
	
	double bbegin, bend, send, begin, end;
	
	#ifdef GENETIC_DEBUG
	cout << "RKGA - Generating initial population (" << pop_size << ")" << endl;
	#endif
	
	begin = now();
	population = population_set(pop_size, chrom_size);
	initialize_population(best);
	end = now();
	initial_time += elapsed_time(begin, end);
	
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
	
	population_set next_gen(pop_size, chrom_size);
	
	bbegin = now();
	for (size_t G = 1; G <= N_GEN; ++G) {
		#ifdef GENETIC_DEBUG
		cout << "RKGA - Generating " << G << "-th generation" << endl;
		print_current_population();
		cout << "    * Generating mutants..." << endl;
		#endif
		
		size_t m = 0;
		begin = now();
		generate_mutants(best, 0, N_MUTANT, next_gen, m);
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
		send = now();
		
		#ifdef GENETIC_DEBUG
		cout << endl;
		#endif
		
		#ifdef GENETIC_VERBOSE
		double best_fit = get_best_individual().get_fitness();
		double etime = elapsed_time(bbegin, send);
		
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
	total_time += elapsed_time(bbegin, bend);
	
	#ifdef GENETIC_DEBUG
	cout << "Generated all generations in " << total_time << " s" << endl;
	#endif
	
	const individual& fittest_individual = get_best_individual();
	current_best_f = best->decode(fittest_individual.get_chromosome());
	
	return true;
}

void rkga::print_performance() const {
	cout << "RKGA algorithm performance:" << endl;
	cout << "    Total generation average:          " << total_time << endl;
	cout << "    Average generation average:        " << total_time/N_GEN << endl;
	cout << "    Total mutant generation time:      " << mutant_time << endl;
	cout << "    Average mutant generation time:    " << mutant_time/N_GEN << endl;
	cout << "    Total crossover generation time:   " << crossover_time << endl;
	cout << "    Average crossover generation time: " << crossover_time/N_GEN << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

