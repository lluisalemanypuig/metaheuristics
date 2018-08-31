#include "solver/interface.hpp"

/// PROTECTED

/// Constraint checking

bool interface::separated_by_D(size_t loc_idx) const {
	const point& FOCUS = locations[loc_idx];
	bool separated = true;
	
	// check for every location ...
	size_t l = 0;
	while (l < n_locations and separated) {
		
		// ... that is not 'loc_idx' and has a centre installed ...
		if (l != loc_idx and location_centre_type[l] != -1) {
			
			// ... that is not at least at a distance of D to 'loc_idx' ...
			if (locations[l].distance(FOCUS) < D) {
				
				// ... then 'loc_idx' is not separated enough from the other
				// locations with a centre installed
				separated = false;
			}
		}
		++l;
	}
	
	return separated;
}

bool interface::separated_by_D(size_t loc_idx, const set<size_t>& locs) const {
	const point& FOCUS = locations[loc_idx];
	bool separated = true;
	
	// check for every location in 'locs' ...
	set<size_t>::const_iterator cit = locs.begin();
	while (cit != locs.end() and separated) {
		
		// ... that is not at least at a distance of D to 'loc_idx' ...
		if (*cit != loc_idx and locations[*cit].distance(FOCUS) < D) {
			
			// ... then 'loc_idx' is not separated enough from the other
			// locations with a centre installed
			separated = false;
		}
		++cit;
	}
	
	return separated;
}

bool interface::working_distance_exceeded(size_t loc_idx, size_t centre_idx) const {
	const double wd = centres[centre_idx].get_working_distance();
	const point& loc_pos = locations[loc_idx];
	
	bool distance_execeeded = false;
	
	size_t city_idx = 0;
	while (city_idx < n_cities and not distance_execeeded) {
		if (cit_by_prim[city_idx] == loc_idx or cit_by_sec[city_idx] == loc_idx) {
			
			const point& city_pos = cities[city_idx].get_position();
			double df;
			
			if (cit_by_prim[city_idx] == loc_idx) {
				df = 1.0;
			}
			else if (cit_by_sec[city_idx] == loc_idx) {
				df = 3.0;
			}
			
			double dist = city_pos.distance(loc_pos);
			if (dist > df*wd) {
				distance_execeeded = true;
			}
		}
		
		++city_idx;
	}
	
	return distance_execeeded;
}

bool interface::capacity_exceeded(size_t loc_idx, size_t centre_idx) const {
	bool cap_exceeded = false;
	
	double max_cap = centres[centre_idx].get_capacity();
	double loc_cap = 0.0;
	
	size_t city_idx = 0;
	while (city_idx < n_cities and not cap_exceeded) {
		if (cit_by_prim[city_idx] == loc_idx) {
			loc_cap += cities[city_idx].get_population();
		}
		else if (cit_by_sec[city_idx] == loc_idx) {
			loc_cap += 0.10*cities[city_idx].get_population();
		}
		
		if (loc_cap > max_cap) {
			cap_exceeded = true;
		}
		
		++city_idx;
	}
	
	return cap_exceeded;
}

bool interface::joined_constraints_satisfied(size_t loc_idx, size_t centre_idx) const {
	const double max_cap = centres[centre_idx].get_capacity();
	const double wd = centres[centre_idx].get_working_distance();
	const point& loc_pos = locations[loc_idx];
	
	double loc_cap = 0.0;
	bool constraints_satisfied = true;
	
	size_t city_idx = 0;
	while (city_idx < n_cities and constraints_satisfied) {
		
		if (cit_by_prim[city_idx] == loc_idx or cit_by_sec[city_idx] == loc_idx) {
			
			const point& city_pos = cities[city_idx].get_position();
			double df;
			
			if (cit_by_prim[city_idx] == loc_idx) {
				loc_cap += cities[city_idx].get_population();
				df = 1.0;
			}
			else if (cit_by_sec[city_idx] == loc_idx) {
				loc_cap += 0.10*cities[city_idx].get_population();
				df = 3.0;
			}
			
			double dist = city_pos.distance(loc_pos);
			if (dist > df*wd or loc_cap > max_cap) {
				constraints_satisfied = false;
			}
		}
		
		++city_idx;
	}
	
	return constraints_satisfied;
}

