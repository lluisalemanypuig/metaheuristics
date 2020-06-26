#include "solver.hpp"

/// NON-CLASS PRIVATE

typedef pair<double, size_t> sorted_chromosome;

/* PUBLIC */

double solver::decode(const chromosome& c) noexcept(false) {
	const size_t n_genes = c.size();
	
	vector<sorted_chromosome> vc(n_genes);
	for (size_t g = 0; g < n_genes; ++g) {
		vc[g].first = c[g];
		vc[g].second = g;
	}
	sort(vc.begin(), vc.end());
	
	set<size_t> used_locations;
	size_t K = 0;
	while (K < 2*n_cities) {
		
		for (size_t vc_idx = 0; vc_idx < n_cities; ++vc_idx) {
			
			greedy_candidate city_and_loc(n_cities, location_role(n_locations, none));
			double candidate_min_cost = numeric_limits<double>::max();
			double dist_min_can = numeric_limits<double>::max();
			
			size_t city_idx = vc[vc_idx].second;
			for (size_t loc_idx = 0; loc_idx < n_locations; ++loc_idx) {
				
				if (not (cit_by_prim[city_idx] == loc_idx or cit_by_sec[city_idx] == loc_idx)) {
					if (separated_by_D(loc_idx, used_locations)) {
						double dist_in_new_candidate = dist_city_loc(city_idx, loc_idx);
						
						if (cit_by_prim[city_idx] == -1) {
							greedy_candidate new_prim_can(city_idx, location_role(loc_idx, primary));
							double pgc = greedy_cost_dist_pop(city_idx, loc_idx, primary);
							get_min_candidate
							(
								city_and_loc, candidate_min_cost, dist_min_can,
								new_prim_can, pgc, dist_in_new_candidate
							);
						}
						
						if (cit_by_sec[city_idx] == -1) {
							greedy_candidate new_sec_can(city_idx, location_role(loc_idx, secondary));
							double sgc = greedy_cost_dist_pop(city_idx, loc_idx, secondary);
							get_min_candidate
							(
								city_and_loc, candidate_min_cost, dist_min_can,
								new_sec_can, sgc, dist_in_new_candidate
							);
						}
					}
				}
			}
			
			size_t chosen_city_idx = city_and_loc.first;
			size_t chosen_loc_idx = city_and_loc.second.first;
			role r = city_and_loc.second.second;
			
			if (chosen_city_idx < n_cities and chosen_loc_idx < n_locations and r != none) {
				if (r == primary) {
					cit_by_prim[chosen_city_idx] = chosen_loc_idx;
					cap_location[chosen_loc_idx] += cities[chosen_city_idx].get_population();
				}
				else if (r == secondary) {
					cit_by_sec[chosen_city_idx] = chosen_loc_idx;
					cap_location[chosen_loc_idx] += 0.10*cities[chosen_city_idx].get_population();
				}
				used_locations.insert(used_locations.end(), chosen_loc_idx);
			}
			
		}
		
		++K;
	}
	
	/// CENTRE ASSIGNATION
	
	double solution_cost = 0.0;
	set<size_t>::const_iterator ul_cit;
	
	// for every used location ...
	for (ul_cit = used_locations.begin(); ul_cit != used_locations.end(); ++ul_cit) {
		
		size_t loc_idx = *ul_cit;
		
		// ... find the centre ...
		size_t centre_idx;
		bool centre_found = false;
		
		size_t ct_idx = 0;
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
			message += "double solver::decode()\n";
			message += "    Cannot build solution\n";
			message += "    -> Centre not found for location: " + std::to_string(loc_idx) + "\n";
			throw infeasible_exception(message);
		}
	}
	
	return -solution_cost;
}

