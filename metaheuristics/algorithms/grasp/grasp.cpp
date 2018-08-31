#include "grasp.hpp"

namespace metaheuristics {
namespace algorithms {

// PRIVATE

// PUBLIC

grasp::grasp() : metaheuristic() {
	MAX_ITER_GRASP = MAX_ITER_LOCAL = -1;	// infinite amount of iterations
	alpha = 1.0;	// completely randomized construction
	
	reset_algorithm();
}

grasp::grasp
(
	size_t mg, size_t ml, double a,
	const local_search_policy& lsp,
	random_number_generator *r
)
: metaheuristic(r)
{
	MAX_ITER_GRASP = mg;
	MAX_ITER_LOCAL = ml;
	alpha = a;
	LSP = lsp;
	
	reset_algorithm();
}

grasp::~grasp() { }

// SETTERS

void grasp::set_local_search_policy(const local_search_policy& lsp) {
	LSP = lsp;
}

void grasp::set_max_iterations_grasp(size_t max) {
	MAX_ITER_GRASP = max;
}

void grasp::set_max_iterations_local(size_t max) {
	MAX_ITER_LOCAL = max;
}

void grasp::reset_algorithm() {
	total_time = 0.0;
	construct_time = 0.0;
	local_search_time = 0.0;
}

// GETTERS

double grasp::get_total_time() const {
	return total_time;
}

double grasp::get_construct_time() const {
	return construct_time;
}

double grasp::get_local_search_time() const {
	return local_search_time;
}

local_search_policy grasp::get_local_search_policy() const {
	return LSP;
}

size_t grasp::get_max_iterations_grasp() const {
	return MAX_ITER_GRASP;
}

size_t grasp::get_max_iterations_local() const {
	return MAX_ITER_LOCAL;
}

bool grasp::execute_algorithm(problem *best, double& current_best_f) {
	if (rng == NULL) {
		cerr << "GRASP : Error" << endl;
		cerr << "bool grasp::execute_algorithm - RNG is not set" << endl;
		return false;
	}
	
	reset_algorithm();
	
	timing bbegin, bend, begin, end;
	
	local_search ls(MAX_ITER_LOCAL, LSP);
	current_best_f = -numeric_limits<double>::max();
	
	#ifdef GRASP_DEBUG
	bool sane;
	#endif
	
	#ifdef GRASP_VERBOSE
	cout << setw(8)  << "GRASP"
		 << setw(15) << " "
		 << setw(18) << "Elaps. Time (s)"
	     << setw(18) << "Obj. Function"
	     << setw(12) << "Iter./" << MAX_ITER_GRASP << endl;
	#endif
	
	bbegin = now();
	for (size_t it = 1; it <= MAX_ITER_GRASP; ++it) {
		problem *r = best->empty();
		
		try {
			begin = now();
			double rcc = r->random_construct(rng, alpha);
			end = now();
			construct_time += elapsed_time(begin, end);
			
			#ifdef GRASP_DEBUG
			sane = r->sanity_check();
			if (not sane) {
				cerr << "grasp::execute_algorithm - Sanity check failed on" << endl;
				cerr << "    solution returned by 'random_construct'." << endl;
				r->print("", cerr);
			}
			#endif
			
			#ifdef GRASP_VERBOSE
			if (rcc > current_best_f) {
				cout << setw(8) << "**R.C.";
			}
			else {
				cout << setw(8) << "R.C.";
			}
			
			cout << setw(15) << " "
				 << setw(18) << elapsed_time(bbegin, now())
				 << setw(18) << rcc
				 << setw(12) << it << endl;
			
			#endif
			
			double lsc = rcc;
			begin = now();
			ls.execute_algorithm(r, lsc);
			end = now();
			local_search_time += elapsed_time(begin, end);
			
			#ifdef GRASP_DEBUG
			sane = r->sanity_check();
			if (not sane) {
				cerr << "grasp::execute_algorithm - Sanity check failed on" << endl;
				cerr << "    solution returned by local search algorithm." << endl;
				r->print("", cerr);
			}
			#endif
			
			#ifdef GRASP_VERBOSE
			if (lsc > rcc and lsc != numeric_limits<double>::max()) {
				cout << setw(8) << " ";
			
				if (lsc > current_best_f) {
					cout << setw(15) << "**L.S.";
				}
				else {
					cout << setw(15) << "L.S.";
				}
			
				cout << setw(18) << elapsed_time(bbegin, now())
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
			//cerr << "bool grasp::execute_algorithm: caught infeasible solution exception in iteration " << it << endl;
			//cerr << e.what() << endl;
			
			end = now();
			construct_time += elapsed_time(begin, end);
			
			cout << setw(8)  << "R.C."
				 << setw(15) << " "
				 << setw(18) << elapsed_time(bbegin, now())
				 << setw(18) << -1
				 << setw(12) << it << endl;
			
			cout << setw(8)  << " "
				 << setw(15) << "L.S."
				 << setw(18) << elapsed_time(bbegin, now())
				 << setw(18) << -1
				 << setw(12) << it << endl;
		}
		
		delete r;
	}
	bend = now();
	total_time = elapsed_time(bbegin, bend);
	
	return true;
}

void grasp::print_performance() const {
	cout << "GRASP metaheuristic performance (for a total of " << MAX_ITER_GRASP << " iterations):" << endl;
	cout << "    Total execution time:      " << total_time << " s" << endl;
	cout << "    Average iteration time:    " << total_time/MAX_ITER_GRASP << " s" << endl;
	cout << "    Total construction time:   " << construct_time << " s" << endl;
	cout << "    Average construction time: " << construct_time/MAX_ITER_GRASP << " s" << endl;
	cout << "    Total local search time:	" << local_search_time << " s" << endl;
	cout << "    Average local search time:	" << local_search_time/MAX_ITER_GRASP << " s" << endl;
	cout << endl;
}

}
}

