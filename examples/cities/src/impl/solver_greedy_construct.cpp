#include "solver.hpp"

/* PUBLIC */

double solver::greedy_construct() noexcept(false) {
	const double inf = numeric_limits<double>::max();
	
	set<int> used_locations;
	int K = 0;
	
	/* LOCATION ASSIGNATION */
	
	while (K < 2*n_cities) {
		
		greedy_candidate city_and_loc(n_cities, location_role(n_locations, none));
		double candidate_min_cost = inf;
		double dist_min_can = inf;
		
		for (int city_idx = 0; city_idx < n_cities; ++city_idx) {
			for (int loc_idx = 0; loc_idx < n_locations; ++loc_idx) {
				
				if (not (cit_by_prim[city_idx] == loc_idx or cit_by_sec[city_idx] == loc_idx)) {
					if (separated_by_D(loc_idx, used_locations)) {
						double dist_new_candidate = dist_city_loc(city_idx, loc_idx);
						
						if (cit_by_prim[city_idx] == -1) {
							greedy_candidate new_can(city_idx, location_role(loc_idx, primary));
							double can_cost = greedy_cost_dist_pop(city_idx, loc_idx, primary);
							if (can_cost < inf) {
								get_min_candidate
								(
									city_and_loc, candidate_min_cost, dist_min_can,
									new_can, can_cost, dist_new_candidate
								);
							}
						}
						
						if (cit_by_sec[city_idx] == -1) {
							greedy_candidate new_can(city_idx, location_role(loc_idx, secondary));
							double can_cost = greedy_cost_dist_pop(city_idx, loc_idx, secondary);
							if (can_cost < inf) {
								get_min_candidate
								(
									city_and_loc, candidate_min_cost, dist_min_can,
									new_can, can_cost, dist_new_candidate
								);
							}
						}
					}
				}
			}
		}
		
		int city_idx = city_and_loc.first;
		int loc_idx = city_and_loc.second.first;
		role r = city_and_loc.second.second;
		
		if (city_idx < n_cities and loc_idx < n_locations and r != none) {
			if (r == primary) {
				cit_by_prim[city_idx] = loc_idx;
				cap_location[loc_idx] += cities[city_idx].get_population();
			}
			else if (r == secondary) {
				cit_by_sec[city_idx] = loc_idx;
				cap_location[loc_idx] += 0.10*cities[city_idx].get_population();
			}
			used_locations.insert(used_locations.end(), loc_idx);
		}
		else {
			string message = "";
			message += "double solver::greedy_construct()\n";
			message += "    Cannot build solution\n";
			message += "    -> Could not find a feasible candidate\n";
			throw infeasible_exception(message);
		}
		
		++K;
	}
	
	/* CENTRE ASSIGNATION */
	
	//cin >> step;
	
	double solution_cost = 0.0;
	set<int>::const_iterator ul_cit;
	
	// for every used location ...
	for (ul_cit = used_locations.begin(); ul_cit != used_locations.end(); ++ul_cit) {
		
		int loc_idx = *ul_cit;
		
		// ... find the centre ...
		int centre_idx;
		bool centre_found = false;
		
		int ct_idx = 0;
		while (ct_idx < n_centres and not centre_found) {
			
			// ... that is the cheapest ...
			centre_idx = sorted_centres[ct_idx];
			
			// ... and that satisfies all constraints ...
			if (joined_constraints_satisfied(loc_idx, centre_idx)) {
				centre_found = true;
			}
			
			++ct_idx;
		}
		
		if (centre_found) {
			location_centre_type[loc_idx] = centre_idx;
			solution_cost += centres[centre_idx].get_installation_cost();
		}
		else {
			string message = "";
			message += "double solver::greedy_construct()\n";
			message += "    Cannot build solution\n";
			message += "    -> Centre not found for location: " + std::to_string(loc_idx) + "\n";
			throw infeasible_exception(message);
		}
	}
	
	return -solution_cost;
}

