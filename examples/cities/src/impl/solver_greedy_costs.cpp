#include "solver.hpp"

/// PRIVATE

void solver::get_min_candidate
(
	greedy_candidate& old_candidate, double& cost_old_can, double& dist_old_can,
	const greedy_candidate& new_candidate, double cost_new_can, double dist_new_can
)
const
{
	// this is a weird way to compute the minimum, but anyway...
	
	bool replace = false;
	if (cost_new_can < cost_old_can) {
		replace = true;
	}
	else if (cost_new_can == cost_old_can and dist_new_can < dist_old_can) {
		replace = true;
	}
	
	if (replace) {
		old_candidate = new_candidate;
		cost_old_can = cost_new_can;
		dist_old_can = dist_new_can;
	}
}

bool solver::at_least_one_centre(int city_idx, int loc_idx, double pf, double df, int& cca) const {
	const double dist = dist_city_loc(city_idx, loc_idx);
	
	bool alo = false;
	int ct_idx = 0;
	while (ct_idx < n_centres and not alo) {
		
		int centre_idx = sorted_centres[ct_idx];
		
		// capacity not exceeded ...
		if (cap_location[loc_idx] + pf*cities[city_idx].get_population() <= centres[centre_idx].get_capacity()) {
			
			// ... and distance not exceeded ...
			if (dist <= df*centres[centre_idx].get_working_distance()) {
				
				// ... a centre was found
				alo = true;
				cca = centre_idx;
			}
		}
		
		++ct_idx;
	}
	
	return alo;
}

double solver::greedy_cost_dist(int city_idx, int loc_idx, role r) const {
	int cca = n_centres;
	
	bool alo;
	if (r == primary) {
		alo = at_least_one_centre(city_idx, loc_idx, 1.0, 1.0, cca);
	}
	else if (r == secondary) {
		alo = at_least_one_centre(city_idx, loc_idx, 0.1, 3.0, cca);
	}
	else {
		cerr << "Error (solver::greedy_cost_dist): invalid role value: " << r << endl;
		alo = false;
	}
	
	char C = 0;
	if (cap_location[loc_idx] == 0) {
		
		// location 'loc_idx' is not serving any city ...
		if (alo) {
			// ... and there is at least one centre type
			// available to serve 'city_idx'
			C = 3;
		}
		else {
			// ... but no centre can serve city 'city_idx'
			C = 2;
		}
	}
	else {
		// location 'loc_idx' is already serving some cities ...
		if (alo) {
			// ... and it can also serve 'city_idx'
			C = 1;
		}
		else {
			// ... and can not serve 'city_idx' 
			C = 2;
		}
	}
	
	double cost = numeric_limits<double>::max();
	
	if (C == 1) {
		cost = dist_city_loc(city_idx, loc_idx);
	}
	else if (C == 3) {
		cost = dist_city_loc(city_idx, loc_idx)*centres[cca].get_installation_cost();
	}
	
	return cost;
}

double solver::greedy_cost_pop(int city_idx, int loc_idx, role r) const {
	int cca = n_centres;
	
	bool alo;
	if (r == primary) {
		alo = at_least_one_centre(city_idx, loc_idx, 1.0, 1.0, cca);
	}
	else if (r == secondary) {
		alo = at_least_one_centre(city_idx, loc_idx, 0.1, 3.0, cca);
	}
	else {
		cerr << "Error (solver::greedy_cost_pop): invalid role value: " << r << endl;
		alo = false;
	}
	
	char C = 0;
	if (cap_location[loc_idx] == 0) {
		
		// location 'loc_idx' is not serving any city ...
		if (alo) {
			// ... and there is at least one centre type
			// available to serve 'city_idx'
			C = 3;
		}
		else {
			// ... but no centre can serve city 'city_idx'
			C = 2;
		}
	}
	else {
		// location 'loc_idx' is already serving some cities ...
		if (alo) {
			// ... and it can also serve 'city_idx'
			C = 1;
		}
		else {
			// ... but can not serve 'city_idx' 
			C = 2;
		}
	}
	
	double cost = numeric_limits<double>::max();
	if (C != 2) {
		
		cost = cap_location[loc_idx];
		if (r == primary) {
			cost += cities[city_idx].get_population();
		}
		else if (r == secondary) {
			cost += 0.10*cities[city_idx].get_population();
		}
		
		if (C == 3) {
			cost *= centres[cca].get_installation_cost();
		}
	}
	return cost;
}

double solver::greedy_cost_dist_pop(int city_idx, int loc_idx, role r) const {
	int cca = n_centres;
	
	bool alo;
	if (r == primary) {
		alo = at_least_one_centre(city_idx, loc_idx, 1.0, 1.0, cca);
	}
	else if (r == secondary) {
		alo = at_least_one_centre(city_idx, loc_idx, 0.1, 3.0, cca);
	}
	else {
		cerr << "Error (solver::greedy_cost_dist_pop): invalid role value: " << r << endl;
		alo = false;
	}
	
	char C = 0;
	if (cap_location[loc_idx] == 0) {
		
		// location 'loc_idx' is not serving any city ...
		if (alo) {
			// ... and there is at least one centre type
			// available to serve 'city_idx'
			C = 3;
		}
		else {
			// ... but no centre can serve city 'city_idx'
			C = 2;
		}
	}
	else {
		// location 'loc_idx' is already serving some cities ...
		if (alo) {
			// ... and it can also serve 'city_idx'
			C = 1;
		}
		else {
			// ... but can not serve 'city_idx' 
			C = 2;
		}
	}
	
	double cost = numeric_limits<double>::max();
	if (C != 2) {
		cost = cap_location[loc_idx];
		if (r == primary) {
			cost += cities[city_idx].get_population();
		}
		else if (r == secondary) {
			cost += 0.10*cities[city_idx].get_population();
		}
		
		if (C == 1) {
			cost += dist_city_loc(city_idx, loc_idx);
		}
		else if (C == 3) {
			cost += dist_city_loc(city_idx, loc_idx)*centres[cca].get_installation_cost();
		}
	}
	return cost;
}

