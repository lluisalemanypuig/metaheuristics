#include "interface.hpp"

// PUBLIC

double interface::evaluate() const {
	double c = 0.0;
	for (size_t l = 0; l < location_centre_type.size(); ++l) {
		if (location_centre_type[l] != -1) {
			c += centres[ location_centre_type[l] ].get_installation_cost();
		}
	}
	return -c;
}

bool interface::sanity_check(ostream& err, const string& tab) const {
	bool is_sane = true;
	
	for (size_t loc_idx = 0; loc_idx < n_locations; ++loc_idx) {
		size_t centre_idx = location_centre_type[loc_idx];
		if (centre_idx != -1) {
			
			if (capacity_exceeded(loc_idx, centre_idx)) {
				is_sane = false;
				err << tab
				    << "(1) Sanity check: capacity constraint unsatisfied"
				    << " in location " << loc_idx << ". Centre installed: "
				    << centre_idx << "." << endl;
			}
			
			if (working_distance_exceeded(loc_idx, centre_idx)) {
				is_sane = false;
				err << tab
				    << "(2) Sanity check: working distance constraint unsatisfied"
				    << " in location " << loc_idx << ". Centre installed: "
				    << centre_idx << "." << endl;
			}
		}
	}
	
	for (size_t l1 = 0; l1 < n_locations; ++l1) {	
		for (size_t l2 = l1 + 1; l2 < n_locations; ++l2) {
			if (location_centre_type[l1] != -1 and location_centre_type[l2] != -1) {
				
				if (locations[l1].distance(locations[l2]) < D) {
					is_sane = false;
					err << tab
						<< "(3) Sanity check: separating distance constraint not satisfied by locations "
						<< l1 << " and " << l2 << "." << endl;
				}
			}
		}
	}
	
	for (size_t c = 0; c < n_cities; ++c) {
		
		if (cit_by_prim[c] == -1 and cit_by_sec[c] == -1) {
			is_sane = false;
			err << tab << "(4) City " << c << " lacks a primary and a secondary location" << endl;
		}
		else if (cit_by_prim[c] == -1) {
			is_sane = false;
			err << tab << "(5) City " << c << " is not served by a primary location" << endl;
		}
		else if (cit_by_sec[c] == -1) {
			is_sane = false;
			err << tab << "(6) City " << c << " is not served by a secondary location" << endl;
		}
		else if (cit_by_prim[c] == cit_by_sec[c]) {
			is_sane = false;
			err << tab
				<< "(7) City " << c << " is served twice by the same location: "
				<< cit_by_prim[c] << endl;
		}
		else if (cit_by_prim[c] != -1 and cit_by_sec[c] != -1) {
			
			if (location_centre_type[ cit_by_prim[c] ] == -1) {
				is_sane = false;
				err << tab
					<< "(8) City " << c << " is served by primary location " << cit_by_prim[c]
					<< " that does not have a centre assigned" << endl;
			}
			
			if (location_centre_type[ cit_by_sec[c] ] == -1) {
				is_sane = false;
				err << tab
					<< "(9) City " << c << " is served by secondary location " << cit_by_sec[c]
					<< " that does not have a centre assigned" << endl;
			}
		}
	}
	
	return is_sane;
}

// PROTECTED

void interface::cities_served_by_location(size_t loc_idx, vector<role>& cities_served, size_t& how_many) const {
	how_many = 0;
	for (size_t c = 0; c < cities.size(); ++c) {
		cities_served[c] = none;

		if (cit_by_prim[c] == loc_idx) {
			cities_served[c] = primary;
			++how_many;
		}
		else if (cit_by_sec[c] == loc_idx) {
			cities_served[c] = secondary;
			++how_many;
		}
	}
}

double interface::dist_city_loc(size_t city_idx, size_t loc_idx) const {
	return cities[city_idx].get_position().distance(locations[loc_idx]);
}
