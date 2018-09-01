#include <metaheuristics/algorithms/local_search/local_search.hpp>

namespace metaheuristics {
namespace algorithms {

// PRIVATE

// PUBLIC

local_search::local_search() : metaheuristic() {
	MAX_ITER = -1; // infinite amount of iterations
	
	reset_algorithm();
}

local_search::local_search(size_t max, const local_search_policy& lsp) : metaheuristic() {
	MAX_ITER = max;
	LSP = lsp;
	reset_algorithm();
}

local_search::~local_search() { }

// SETTERS

void local_search::set_local_search_policy(const local_search_policy& lsp) {
	LSP = lsp;
}

void local_search::set_max_iterations(size_t max) {
	MAX_ITER = max;
}

void local_search::reset_algorithm() {
	total_time = 0.0;
	neighbourhood_time = 0.0;
	ITERATION = 1;
}

// GETTERS

double local_search::get_total_time() const {
	return total_time;
}

double local_search::get_neighbourhood_time() const {
	return neighbourhood_time;
}

size_t local_search::get_n_iterations() const {
	return ITERATION;
}

local_search_policy local_search::get_local_search_policy() const {
	return LSP;
}

size_t local_search::get_max_iterations() const {
	return MAX_ITER;
}

bool local_search::execute_algorithm(problem *best, double& current_best_f) {
	reset_algorithm();
	
	#ifdef LOCAL_SEARCH_VERBOSE
	cout << setw(8)  << " "
		 << setw(15) << "Local Search"
		 << setw(18) << "Elaps. Time (s)"
	     << setw(18) << "Obj. Function"
	     << setw(12) << "Iter./" << MAX_ITER
	     << setw(25) << "# Neighbours explored"
	     << endl;
	
	cout << setw(8)  << " "
		 << setw(15) << " "
		 << setw(18) << 0.0
	     << setw(18) << current_best_f
	     << setw(12) << 0
	     << setw(25) << 0
	     << endl;
	#endif
	
	double bbegin, bend, begin, end;
	
	bool improvement = true;
	
	bbegin = now();
	while (ITERATION <= MAX_ITER and improvement) {
		pair<problem *, double> neighbour;
		neighbour.first = NULL;
		neighbour.second = current_best_f;
		
		begin = now();
		best->best_neighbour(neighbour, LSP);
		end = now();
		neighbourhood_time += elapsed_time(begin, end);
		
		if (neighbour.first != NULL) {
			
			#ifdef LOCAL_SEARCH_DEBUG
			bool sane = best->sanity_check();
			if (not sane) {
				cerr << "local_search::execute_algorithm - Sanity check failed on" << endl;
				cerr << "    solution returned by 'best_neighbour'." << endl;
				best->print("", cerr);
			}
			#endif
			
			if (neighbour.second > current_best_f) {
				current_best_f = neighbour.second;
				
				#ifdef LOCAL_SEARCH_VERBOSE
				cout << setw(8)  << " "
					 << setw(15) << " "
					 << setw(18) << elapsed_time(bbegin, now())
					 << setw(18) << current_best_f
					 << setw(12) << ITERATION
					 << setw(25) << best->get_n_neighbours_explored()
					 << endl;
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
	bend = now();
	total_time = elapsed_time(bbegin, bend);
	
	return true;
}

void local_search::print_performance() const {
	cout << "Local Search algorithm performance:" << endl;
	cout << "    Number of iterations:                   " << ITERATION << endl;
	cout << "    Total execution time:                   " << total_time << " s" << endl;
	cout << "    Average iteration time:                 " << total_time/ITERATION << " s" << endl;
	cout << "    Total neihgbourhood exploration time:   " << neighbourhood_time << " s" << endl;
	cout << "    Average neihgbourhood exploration time: " << neighbourhood_time/ITERATION << " s" << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

