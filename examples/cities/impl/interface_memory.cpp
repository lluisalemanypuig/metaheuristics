#include "solver/interface.hpp"

/// PROTECTED

void interface::copy_interface(const interface *i) {
	D = i->D;
	n_locations = i->n_locations;
	n_cities = i->n_cities;
	n_centres = i->n_centres;
	
	locations = i->locations;
	cities = i->cities;
	centres = i->centres;
	
	sorted_cities = i->sorted_cities;
	sorted_centres = i->sorted_centres;
	
	location_centre_type = i->location_centre_type;
	cap_location = i->cap_location;
	cit_by_prim = i->cit_by_prim;
	cit_by_sec = i->cit_by_sec;
}

void interface::copy_empty(const interface *i) {
	D = i->D;
	n_locations = i->n_locations;
	n_cities = i->n_cities;
	n_centres = i->n_centres;
	
	locations = i->locations;
	cities = i->cities;
	centres = i->centres;
	
	sorted_cities = i->sorted_cities;
	sorted_centres = i->sorted_centres;
	
	location_centre_type = vector<int>(n_locations, -1);
	cap_location = vector<double>(n_locations, 0.0);
	cit_by_prim = vector<int>(n_cities, -1);
	cit_by_sec = vector<int>(n_cities, -1);
}

void interface::empty_interface() {
	location_centre_type = vector<int>(n_locations, -1);
	cap_location = vector<double>(n_locations, 0.0);
	cit_by_prim = vector<int>(n_cities, -1);
	cit_by_sec = vector<int>(n_cities, -1);
}

/// PUBLIC

void interface::clear() {
	empty_interface();
}

