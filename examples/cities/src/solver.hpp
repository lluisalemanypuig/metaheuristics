#pragma once

// C++ includes
#include <algorithm>
#include <iostream>
#include <queue>
#include <set>
using namespace std;

// metaheuristics includes
#include <metaheuristics/structures.hpp>
using namespace metaheuristics;
using namespace random;

// Custom includes
#include "interface.hpp"

class solver : public interface {
	public:
		solver() = default;
		~solver() = default;
		
		// ----------------------------
		// -- PROBLEM IMPLEMENTATION --
		// ----------------------------
		
		/* Constructing a solution */
		
		// Constructs an empty solution.
		// Calls constructor with no parameters.
		problem<> *empty() const;
		
		// Constructs a solution from scratch. It may use a greedy/random/... algorithm
		// Returns the evaluation of the solution.
		double greedy_construct() noexcept(false);
		
		// Constructs a randomized solution using a restricted candidate
		// list (RCL) sorted using the parameter alpha.
		double random_construct(drandom_generator<> *rng, double alpha) noexcept(false);
		
		// Constructs a solution from a given chromosome.
		// Returns the evaluation of the solution.
		double decode(const chromosome& c) noexcept(false);
		
		// Explores this solution's neighbourhood and stores:
		// - the best neighbour if BI is true (best improvement)
		// - the first best neighbour if BI is false (first improvement)
		// Best means a neighbour that maximizes the evaluate() function.
		void best_neighbour(pair<problem<>*, double>& best_neighbour, const local_search_policy& lsp = Best_Improvement);
		
		/* Memory handling */
		
		// Creates a copy of the instance of this problem.
		problem<> *clone() const;
		
		// Creates a copy of the instance of the problem passed as parameter.
		void copy(const problem<> *p);
		
		// Clears the memory used by the instance of this problem.
		// Everything is reset so that the method construct would create
		// a solution to the problem if it were to be called.
		void clear();
		
	private:

		/* GREEDY CONSTRUCTOR */

		typedef pair<int, interface::role> location_role;
		typedef pair<int, location_role> greedy_candidate;

		// Compares the new candidate to the current one and replaces the
		// current if the new is better.
		void get_min_candidate
		(
			greedy_candidate& city_and_loc, double& candidate_min_cost, double& dist_min_can,
			const greedy_candidate& new_candidate, double cost_new_can, double dist_in_new_can
		)
		const;

		// Auxiliar function: returns whether there is at least one centre type
		// available for location 'loc_idx' that can serve the cities already
		// being served plus city 'city_idx'.
		// pf and df are the population and distance factors that are:
		// -> (1.0, 1.0) if the role is primary
		// -> (0.1, 3.0) if the role is secondary
		// In cca is stored the index of the cheapest centre available for
		// location 'loc_idx'.
		bool at_least_one_centre(int city_idx, int loc_idx, double pf, double df, int& cca) const;

		// Returns the greedy cost of assigning 'loc_idx'
		// to city 'city_idx' with role 'role' (primary if role = true)
		double greedy_cost_dist(int city_idx, int loc_idx, role r) const;
		double greedy_cost_pop(int city_idx, int loc_idx, role r) const;
		double greedy_cost_dist_pop(int city_idx, int loc_idx, role r) const;

		/* NEIGHBOURHOOD EXPLORATION */

		// Can 'loc_idx' location stop serving its cities 'cities_served'?
		// That is, are there other locations with a centre installed
		// in them, other than 'loc_idx' to serve the cities in
		// 'cities_served' ** with the same role ** ?
		// Store these new locations in 'what_locs'.
		// Remember:
		//   what_locs[c] = -> invalid location index if cities_served[c] = 0
		//                  -> valid location index if otherwise
		bool can_remove_centre
		(
			int loc_idx,
			const vector<role>& cities_served, vector<int>& what_locs
		)
		const;

		// Deletes the centre from location 'loc_idx' and updates all data
		// structures. The new solution is stored in 'neigh'.
		void delete_centre
		(
			int loc_idx, const vector<role>& cities_served,
			const vector<int>& new_locations, problem **neigh
		)
		const;

		// Assuming that location 'loc_idx' has a centre installed, find
		// a centre that has strictly a lower installation cost that can
		// replace the one already installed in that location.
		bool find_better_centre(int loc_idx, int& centre_idx) const;

		// Replaces the centre from location 'l' and updates all data
		// structures. The new solution is stored in 'neigh'.
		void replace_centre(int loc_idx, int centre_idx, problem **neigh) const;
};

