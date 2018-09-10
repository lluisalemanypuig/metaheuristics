#include <metaheuristics/algorithms/local_search/local_search.hpp>

namespace metaheuristics {
namespace algorithms {

// PRIVATE

// PUBLIC

template<class G, typename dT>
local_search<G,dT>::local_search() : metaheuristic<G,dT>() {
	MAX_ITER = -1; // infinite amount of iterations
	
	reset_algorithm();
}

template<class G, typename dT>
local_search<G,dT>::local_search(size_t max, const local_search_policy& lsp) : metaheuristic<G,dT>() {
	MAX_ITER = max;
	LSP = lsp;
	reset_algorithm();
}

template<class G, typename dT>
local_search<G,dT>::~local_search() { }

// SETTERS

template<class G, typename dT>
void local_search<G,dT>::set_local_search_policy(const local_search_policy& lsp) {
	LSP = lsp;
}

template<class G, typename dT>
void local_search<G,dT>::set_max_iterations(size_t max) {
	MAX_ITER = max;
}

template<class G, typename dT>
void local_search<G,dT>::reset_algorithm() {
	total_time = 0.0;
	neighbourhood_time = 0.0;
	ITERATION = 1;
}

// GETTERS

template<class G, typename dT>
double local_search<G,dT>::get_total_time() const {
	return total_time;
}

template<class G, typename dT>
double local_search<G,dT>::get_neighbourhood_time() const {
	return neighbourhood_time;
}

template<class G, typename dT>
size_t local_search<G,dT>::get_n_iterations() const {
	return ITERATION;
}

template<class G, typename dT>
local_search_policy local_search<G,dT>::get_local_search_policy() const {
	return LSP;
}

template<class G, typename dT>
size_t local_search<G,dT>::get_max_iterations() const {
	return MAX_ITER;
}

template<class G, typename dT>
bool local_search<G,dT>::execute_algorithm(problem<G,dT> *best, double& current_best_f) {
	// set the algorithm to its initial state
	reset_algorithm();
	
	#if defined (LOCAL_SEARCH_VERBOSE)
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
	
	// 
	time_point bbegin, bend, begin, end;
	
	bool improvement = true;
	
	bbegin = now();
	while (ITERATION <= MAX_ITER and improvement) {
		pair<problem<G,dT>*, double> neighbour;
		neighbour.first = NULL;
		neighbour.second = current_best_f;
		
		begin = now();
		best->best_neighbour(neighbour, LSP);
		end = now();
		neighbourhood_time += elapsed_seconds(begin, end);
		
		if (neighbour.first != NULL) {
			
			#if defined (LOCAL_SEARCH_debug)
				if (not best->sanity_check()) {
					cerr << "local_search<G,dT>::execute_algorithm - Sanity check failed on" << endl;
					cerr << "    solution returned by 'best_neighbour'." << endl;
					best->print("", cerr);
				}
			#endif
			
			if (neighbour.second > current_best_f) {
				current_best_f = neighbour.second;
				
				#if defined (LOCAL_SEARCH_VERBOSE)
					cout << setw(8)  << " "
						 << setw(15) << " "
						 << setw(18) << elapsed_seconds(bbegin, now())
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
	total_time = elapsed_seconds(bbegin, bend);
	
	return true;
}

template<class G, typename dT>
void local_search<G,dT>::print_performance() const {
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

