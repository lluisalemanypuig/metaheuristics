#pragma once

// C++ includes
#include <iostream>
#include <iomanip>
#include <limits>
using namespace std;

// Custom includes
#include "algorithms/local_search/local_search.hpp"
#include "algorithms/metaheuristic.hpp"
#include "algorithms/problem.hpp"

namespace metaheuristics {
namespace algorithms {

class grasp : public metaheuristic {
	private:
		double total_time, construct_time, local_search_time;
		
		size_t MAX_ITER_GRASP, MAX_ITER_LOCAL;
		double alpha;
		local_search_policy LSP;
		
	public:
		grasp();
		grasp
		(
			size_t MAX_ITER_GRASP,
			size_t MAX_ITER_LOCAL,
			double alpha,
			const local_search_policy& lsp,
			random_number_generator *rng
		);
		~grasp();
		
		// SETTERS
		
		void set_local_search_policy(const local_search_policy& lsp);
		void set_max_iterations_grasp(size_t max);
		void set_max_iterations_local(size_t max);
		void reset_algorithm();
		
		// GETTERS
		
		double get_total_time() const;
		double get_construct_time() const;
		double get_local_search_time() const;
		
		local_search_policy get_local_search_policy() const;
		size_t get_max_iterations_grasp() const;
		size_t get_max_iterations_local() const;
		
		// pre: p is an empty instance of problem
		// post: p is a solution to the problem with a certain cost
		bool execute_algorithm(problem *p, double& c);
		
		void print_performance() const;
};

}
}

