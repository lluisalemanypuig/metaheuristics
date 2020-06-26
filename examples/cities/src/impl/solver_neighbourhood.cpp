#include "solver.hpp"

/// NON-CLASS PRIVATE

typedef pair<size_t, size_t> candidate;

inline
bool __satisfies_constraints
(
	double current_capacity, double added_capacity, double centre_cap,
	double dist_loc_city, double max_dist
)
{
	// check capacity constraint first ...
	double expected_capacity = current_capacity + added_capacity;
	bool sat = expected_capacity <= centre_cap;
	
	if (sat) {
		
		// ... and if it is satisfied, check working distance constraint
		sat = dist_loc_city <= max_dist;
	}
	
	return sat;
}

/// PRIVATE

bool solver::can_remove_centre
(
	size_t loc_idx, const vector<role>& cities_served, vector<size_t>& new_locs
)
const
{
	vector<pair<double, size_t> > sorted_cities_served(n_cities);
	for (size_t c = 0; c < n_cities; ++c) {
		if (cities_served[c] == none) {
			sorted_cities_served[c].first = numeric_limits<double>::max();
		}
		else {
			sorted_cities_served[c].first = cities[c].get_population();
			if (cities_served[c] == secondary) {
				sorted_cities_served[c].first *= 0.1;
			}
		}
		sorted_cities_served[c].second = c;
	}
	sort(sorted_cities_served.begin(), sorted_cities_served.end());
	
	// cities that were served by 'loc_idx' are now sorted increasingly
	// by their added capacity
	
	
	bool can_remove = true;
	vector<double> copy_cap_location = cap_location;
	
	// for every city ...
	size_t c_idx = 0;
	while (c_idx < n_cities and can_remove) {
		
		size_t city_idx = sorted_cities_served[c_idx].second;
		const point& city_pos = cities[city_idx].get_position();
		new_locs[city_idx] = n_locations;
		
		// ... served by location 'loc_idx' ...
		if (cities_served[city_idx] != none)  {
			
			double max_gap = 0.0;	// gap = |cap_location + city_pop - centre_capacity|
			double min_dist = numeric_limits<double>::max();	// distance between 'city_idx' and location 'l_it'
			size_t min_loc = n_locations;
			
			// ... check every location ...
			size_t l_it = 0;
			while (l_it < n_locations) {
				
				// ... that is not 'loc_idx', has a centre installed in it ...
				if (l_it != loc_idx and location_centre_type[l_it] != -1) {
					// ... and is not serving already city 'c' ...
					if (cit_by_prim[city_idx] != l_it and cit_by_sec[city_idx] != l_it) {
						
						double location_capacity = copy_cap_location[l_it];	// current capacity occupied at location l_it
						double city_pop = cities[city_idx].get_population();	// added capacity
						double dist_city_loc = city_pos.distance(locations[l_it]);	// distance between location and city
						
						size_t centre_idx = location_centre_type[l_it];
						double centre_cap = centres[centre_idx].get_capacity();// centre capacity
						double wd = centres[centre_idx].get_working_distance();// centre's working distance
						
						// population and distance factors
						double pf, df;
						if (cities_served[city_idx] == primary) {
							pf = 1.0;
							df = 1.0;
						}
						else if (cities_served[city_idx] == secondary) {
							pf = 0.1;
							df = 3.0;
						}
						else {
							cerr << "Error (solver::can_remove_centre): city " << city_idx << " not served by a location" << endl;
							df = pf = -1.0;
						}
						
						// ... check whether it satisfies all constraints or not ...
						if ((location_capacity + pf*city_pop) <= centre_cap and dist_city_loc <= df*wd) {
							
							// gap = |cap_location + pf*city_pop - centre_capacity|
							double this_gap = centre_cap - (location_capacity + pf*city_pop);
							
							if ((max_gap < this_gap) or ((max_gap == this_gap) and (dist_city_loc < min_dist))) {
								max_gap = this_gap;
								min_dist = dist_city_loc;
								min_loc = l_it;
							}
						}
					}
				}
				
				++l_it;
			}
			
			if (min_loc < n_locations) {
				new_locs[city_idx] = min_loc;
				
				double pf = (cities_served[city_idx] == primary ? 1.0 : 0.1);
				copy_cap_location[min_loc] += pf*cities[city_idx].get_population();
			}
			else {
				can_remove = false;
			}
		}
		
		++c_idx;
	}
	
	return can_remove;
}

void solver::delete_centre
(
	size_t loc_idx,
	const vector<role>& cities_served,
	const vector<size_t>& new_locations,
	problem **neigh
)
const
{
	*neigh = clone();
	solver *new_sol = static_cast<solver *>(*neigh);
	
	// 1. Uninstall old centre from location 'l'
	new_sol->location_centre_type[loc_idx] = -1;
	
	// 2. Update location capacities:
	//    2.1. Add the populations of cities served by location 'l'
	//         to the new locations that serve them
	//    2.2. set new locations serving the orphaned cities
	for (size_t city_idx = 0; city_idx < n_cities; ++city_idx) {
		
		size_t new_idx = new_locations[city_idx];
		
		if (cities_served[city_idx] == primary) {
			new_sol->cap_location[new_idx] += cities[city_idx].get_population();
			new_sol->cit_by_prim[city_idx] = new_idx;
		}
		else if (cities_served[city_idx] == secondary) {
			new_sol->cap_location[new_idx] += 0.10*cities[city_idx].get_population();
			new_sol->cit_by_sec[city_idx] = new_idx;
		}
	}
	
	//    2.3. set to 0 the capacity occupied at location 'l'
	new_sol->cap_location[loc_idx] = 0.0;
}

void solver::replace_centre(size_t loc_idx, size_t centre_idx, problem **neigh) const
{
	*neigh = clone();
	solver *new_sol = static_cast<solver *>(*neigh);
	
	// 1. Replace the old centre in location 'loc_idx'
	new_sol->location_centre_type[loc_idx] = centre_idx;
}

bool solver::find_better_centre(size_t loc_idx, size_t& centre_idx) const {
	size_t centre_installed = location_centre_type[loc_idx];
	double instal_cost = centres[centre_installed].get_installation_cost();
	
	centre_idx = n_centres;
	bool centre_found = false;
	bool cheaper_centres_left = true;
	
	size_t ct_idx = 0;
	while (ct_idx < n_centres and not centre_found and cheaper_centres_left) {
		
		centre_idx = sorted_centres[ct_idx];
		if (centres[centre_idx].get_installation_cost() < instal_cost) {
			if (joined_constraints_satisfied(loc_idx, centre_idx)) {
				centre_found = true;
			}
		}
		else {
			cheaper_centres_left = false;
		}
		++ct_idx;
	}
	
	return centre_found;
}

/* PUBLIC */

void solver::best_neighbour(pair<problem *, double>& bn, const local_search_policy& lsp) {
	n_neighbours_explored = 0;
	
	double original_cost = -bn.second;
	double best_cost = original_cost;
	
	bool finish = false;
	
	// for all locations that have a centre installed
	size_t loc_idx = 0;
	while (loc_idx < n_locations and not finish) {
		
		if (location_centre_type[loc_idx] != -1) {
			// what cities is this location serving?
			size_t n_cities_served;
			vector<role> cities_served(n_cities);
			cities_served_by_location(loc_idx, cities_served, n_cities_served);
			
			// can we remove the centre? ...
			vector<size_t> new_locations(n_cities);
			bool can_remove = can_remove_centre(loc_idx, cities_served, new_locations);
			
			if (can_remove) {
				// ... if so, check that the new solution is indeed better
				// than the current best one found
				++n_neighbours_explored;
				
				double centre_cost = centres[location_centre_type[loc_idx]].get_installation_cost();
				if (original_cost - centre_cost < best_cost) {
					
					if (lsp == First_Improvement) {
						finish = true;
					}
					
					problem *neigh;
					delete_centre(loc_idx, cities_served, new_locations, &neigh);
					
					if (bn.first != NULL) {
						delete bn.first;
					}
					
					bn.first = neigh;
					best_cost = original_cost - centre_cost;
				}
			}
			else {
				// ... we can not remove it ...
				size_t cheaper_centre;
				bool can_replace = find_better_centre(loc_idx, cheaper_centre);
				
				// ... but can we replace it with a cheaper one?
				if (can_replace) {
					++n_neighbours_explored;
					
					double old_centre_cost = centres[location_centre_type[loc_idx]].get_installation_cost();
					double new_centre_cost = centres[cheaper_centre].get_installation_cost();
					
					// ... if so, check that the new solution is indeed better than
					// the current best one found
					if (original_cost - old_centre_cost + new_centre_cost < best_cost) {
						
						if (lsp == First_Improvement) {
							finish = true;
						}
						
						problem *neigh;
						replace_centre(loc_idx, cheaper_centre, &neigh);
						
						if (bn.first != NULL) {
							delete bn.first;
						}
						
						bn.first = neigh;
						best_cost = original_cost - old_centre_cost + new_centre_cost;
					}
				}
			}
		}
		
		++loc_idx;
	}
	
	bn.second = -best_cost;
}

