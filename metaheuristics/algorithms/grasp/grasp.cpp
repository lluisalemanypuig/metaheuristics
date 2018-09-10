#include <metaheuristics/algorithms/grasp/grasp.hpp>

namespace metaheuristics {
namespace algorithms {

// PRIVATE

// PUBLIC

template<class G, typename dT>
grasp<G,dT>::grasp() : metaheuristic<G,dT>() {
	MAX_ITER_GRASP = MAX_ITER_LOCAL = -1;	// infinite amount of iterations
	alpha = 1.0;	// completely randomized construction
}

template<class G, typename dT>
grasp<G,dT>::grasp
(
	size_t mg, size_t ml, double a,
	const local_search_policy& lsp
)
: metaheuristic<G,dT>()
{
	MAX_ITER_GRASP = mg;
	MAX_ITER_LOCAL = ml;
	alpha = a;
	LSP = lsp;
}

template<class G, typename dT>
grasp<G,dT>::~grasp() { }

// SETTERS

template<class G, typename dT>
void grasp<G,dT>::set_local_search_policy(const local_search_policy& lsp) {
	LSP = lsp;
}

template<class G, typename dT>
void grasp<G,dT>::set_max_iterations_grasp(size_t max) {
	MAX_ITER_GRASP = max;
}

template<class G, typename dT>
void grasp<G,dT>::set_max_iterations_local(size_t max) {
	MAX_ITER_LOCAL = max;
}

template<class G, typename dT>
void grasp<G,dT>::reset_algorithm() {
	total_time = 0.0;
	construct_time = 0.0;
	local_search_time = 0.0;
}

// GETTERS

template<class G, typename dT>
double grasp<G,dT>::get_total_time() const {
	return total_time;
}

template<class G, typename dT>
double grasp<G,dT>::get_construct_time() const {
	return construct_time;
}

template<class G, typename dT>
double grasp<G,dT>::get_local_search_time() const {
	return local_search_time;
}

template<class G, typename dT>
local_search_policy grasp<G,dT>::get_local_search_policy() const {
	return LSP;
}

template<class G, typename dT>
size_t grasp<G,dT>::get_max_iterations_grasp() const {
	return MAX_ITER_GRASP;
}

template<class G, typename dT>
size_t grasp<G,dT>::get_max_iterations_local() const {
	return MAX_ITER_LOCAL;
}

template<class G, typename dT>
bool grasp<G,dT>::execute_algorithm(problem<G,dT> *best, double& current_best_f) {
	// set algorithm to its initial state
	reset_algorithm();
	
	// timing variables
	time_point bbegin, bend, begin, end;
	
	local_search<G,dT> ls(MAX_ITER_LOCAL, LSP);
	current_best_f = -numeric_limits<double>::max();
	
	#if defined (GRASP_VERBOSE)
		cout << setw(8)  << "GRASP"
			 << setw(15) << " "
			 << setw(18) << "Elaps. Time (s)"
			 << setw(18) << "Obj. Function"
			 << setw(12) << "Iter./" << MAX_ITER_GRASP << endl;
	#endif
	
	bbegin = now();
	for (size_t it = 1; it <= MAX_ITER_GRASP; ++it) {
		problem<G,dT> *r = best->empty();
		
		try {
			begin = now();
			double rcc = r->random_construct(&drng, alpha);
			end = now();
			construct_time += elapsed_seconds(begin, end);
			
			#if defined (GRASP_DEBUG)
				if (not r->sanity_check()) {
					cerr << "grasp<G,dT>::execute_algorithm - Sanity check failed on" << endl;
					cerr << "    solution returned by 'random_construct'." << endl;
					r->print("", cerr);
				}
			#endif
			
			#if defined (GRASP_VERBOSE)
				if (rcc > current_best_f) {
					cout << setw(8) << "**R.C.";
				}
				else {
					cout << setw(8) << "R.C.";
				}
			
				cout << setw(15) << " "
					 << setw(18) << elapsed_seconds(bbegin, now())
					 << setw(18) << rcc
					 << setw(12) << it << endl;
			#endif
			
			double lsc = rcc;
			begin = now();
			ls.execute_algorithm(r, lsc);
			end = now();
			local_search_time += elapsed_seconds(begin, end);
			
			#if defined (GRASP_DEBUG)
				if (not r->sanity_check()) {
					cerr << "grasp<G,dT>::execute_algorithm - Sanity check failed on" << endl;
					cerr << "    solution returned by local search algorithm." << endl;
					r->print("", cerr);
				}
			#endif
			
			#if defined (GRASP_VERBOSE)
				if (lsc > rcc and lsc != numeric_limits<double>::max()) {
					cout << setw(8) << " ";
				
					if (lsc > current_best_f) {
						cout << setw(15) << "**L.S.";
					}
					else {
						cout << setw(15) << "L.S.";
					}
				
					cout << setw(18) << elapsed_seconds(bbegin, now())
						 << setw(18) << lsc
						 << setw(12) << it << endl;
				}
			#endif
			
			if (lsc > current_best_f) {
				current_best_f = lsc;
				best->copy(r);
			}
		}
		catch (const infeasible_exception& e) {
			//cerr << "bool grasp<G,dT>::execute_algorithm: caught infeasible solution exception in iteration " << it << endl;
			//cerr << e.what() << endl;
			
			end = now();
			construct_time += elapsed_seconds(begin, end);
			
			#if defined (GRASP_VERBOSE)
				cout << setw(8)  << "R.C."
					 << setw(15) << " "
					 << setw(18) << elapsed_seconds(bbegin, now())
					 << setw(18) << -1
					 << setw(12) << it << endl;
				
				cout << setw(8)  << " "
					 << setw(15) << "L.S."
					 << setw(18) << elapsed_seconds(bbegin, now())
					 << setw(18) << -1
					 << setw(12) << it << endl;
			#endif
		}
		
		delete r;
	}
	bend = now();
	total_time = elapsed_seconds(bbegin, bend);
	
	return true;
}

template<class G, typename dT>
void grasp<G,dT>::print_performance() const {
	cout << "GRASP metaheuristic performance (for a total of " << MAX_ITER_GRASP << " iterations):" << endl;
	cout << "    Total execution time:      " << total_time << " s" << endl;
	cout << "    Average iteration time:    " << total_time/MAX_ITER_GRASP << " s" << endl;
	cout << "    Total construction time:   " << construct_time << " s" << endl;
	cout << "    Average construction time: " << construct_time/MAX_ITER_GRASP << " s" << endl;
	cout << "    Total local search time:	" << local_search_time << " s" << endl;
	cout << "    Average local search time:	" << local_search_time/MAX_ITER_GRASP << " s" << endl;
	cout << endl;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

