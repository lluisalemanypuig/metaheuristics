#include "solver.hpp"

/// NON-CLASS PRIVATE

typedef pair<int, interface::role> locR;
typedef pair<int, locR> candidate;
typedef pair<double, candidate> eval_candidate;
typedef set<eval_candidate>::const_iterator scit;
typedef vector<eval_candidate>::const_iterator vcit;

/* PUBLIC */

double solver::random_construct(drandom_generator<> *RNG, double alpha) noexcept(false) {
	set<int> used_locations;
	
	// LOCATION ASSIGNATION
	
	set<eval_candidate> candidate_list;
	
	int K = 0;
	while (K < 2*n_cities) {
		
		// build candidate list
		candidate_list.clear();
		
		for (int city_idx = 0; city_idx < n_cities; ++city_idx) {
			for (int loc_idx = 0; loc_idx < n_locations; ++loc_idx) {
				
				if (not (cit_by_prim[city_idx] == loc_idx or cit_by_sec[city_idx] == loc_idx)) {
					if (separated_by_D(loc_idx, used_locations)) {
						//double dist_in_new_candidate = dist_city_loc(city_idx, loc_idx);
						
						if (cit_by_prim[city_idx] == -1) {
							candidate new_prim_can(city_idx, locR(loc_idx, primary));
							double pgc = greedy_cost_dist_pop(city_idx, loc_idx, primary);
							candidate_list.insert( eval_candidate(pgc, new_prim_can) );
						}
						
						if (cit_by_sec[city_idx] == -1) {
							candidate new_sec_can(city_idx, locR(loc_idx, secondary));
							double sgc = greedy_cost_dist_pop(city_idx, loc_idx, secondary);
							candidate_list.insert( eval_candidate(sgc, new_sec_can) );
						}
					}
				}
			}
		}
		
		double min_cost = candidate_list.begin()->first;
		scit last = candidate_list.end(); --last;
		double max_cost = last->first;
		
		// build the restricted candidate list
		vector<eval_candidate> RCL;
		for (scit it = candidate_list.begin(); it != candidate_list.end(); ++it) {
			double cost = it->first;
			if (cost <= min_cost + alpha*(max_cost - min_cost)) {
				RCL.push_back(*it);
			}
		}
		
		// take a candidate at random
		RNG->init_uniform(0, RCL.size() - 1);
		int RCL_idx = static_cast<int>(RNG->get_uniform());
		const eval_candidate& C = RCL[RCL_idx];
		
		const candidate& can = C.second;
		int city_idx = can.first;
		int loc_idx = can.second.first;
		role r = can.second.second;
		
		if (r == primary) {
			cit_by_prim[city_idx] = loc_idx;
		}
		else if (r == secondary) {
			cit_by_sec[city_idx] = loc_idx;
		}
		
		double f = 1.0;
		if (r == secondary) {
			f = 0.1;
		}
		
		cap_location[loc_idx] += f*cities[city_idx].get_population();
		used_locations.insert(loc_idx);
		
		++K;
	}
	
	/// CENTRE ASSIGNATION
	
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
			message += "double solver::random_construct()\n";
			message += "    Cannot build solution\n";
			message += "    -> Centre not found for location: " + std::to_string(loc_idx) + "\n";
			throw infeasible_exception(message);
		}
	}
	
	return -solution_cost;
}

