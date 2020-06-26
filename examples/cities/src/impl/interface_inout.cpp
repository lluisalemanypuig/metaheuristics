#include "interface.hpp"

// auxiliary functions

inline
void __read_line_values(istream& in, vector<double>& values) {
	string field_name;
	char eq, left_bracket, right_bracket, semicolon;
	
	in >> field_name;
	in >> eq >> left_bracket;
	
	for (size_t i = 0; i < values.size(); ++i) {
		in >> values[i];
	}
	
	in >> right_bracket >> semicolon;
}

inline
void __read_line_values(istream& in, vector<size_t>& values) {
	string field_name;
	char eq, left_bracket, right_bracket, semicolon;
	
	in >> field_name;
	in >> eq >> left_bracket;
	
	for (size_t i = 0; i < values.size(); ++i) {
		in >> values[i];
	}
	
	in >> right_bracket >> semicolon;
}

// PUBLIC

void interface::read_from_file(istream& in) {
	string field;
	char eq, semicolon;
	
	vector<double> locations_x, locations_y;
	vector<double> city_x, city_y;
	vector<size_t> city_pop;
	
	vector<double> centres_wd, centres_ic;
	vector<size_t> centres_cc;

/*
/// 1
D			= 1;

/// 2
nLocations	= 2;			// 2.1
loc_x		= [1 4];		// 2.2
loc_y		= [6 3];		// 2.3

/// 3
nCities		= 3;			// 3.1
city_x		= [1 4 6];		// 3.2
city_y		= [1 2 3];		// 3.3
city_pop	= [1 2 3];		// 3.4

/// 4
nCentres	= 4;			// 4.1
work_dist	= [1 2 3 4];	// 4.2
centre_cap	= [1 2 3 4];	// 4.3
instal_cost	= [1 2 3 4];	// 4.4
*/

	/// 1
	in >> field;
	in >> eq;
	in >> D;
	in >> semicolon;
	
	/// 2
	// 2.1
	in >> field;
	in >> eq;
	in >> n_locations;
	in >> semicolon;
	
	locations_x.resize(n_locations);
	locations_y.resize(n_locations);
	
	__read_line_values(in, locations_x);	// 2.2
	__read_line_values(in, locations_y);	// 2.3
	
	/// 3
	// 3.1
	in >> field;
	in >> eq;
	in >> n_cities;
	in >> semicolon;
	
	city_x.resize(n_cities);
	city_y.resize(n_cities);
	city_pop.resize(n_cities);
	
	__read_line_values(in, city_x);	// 3.2
	__read_line_values(in, city_y);	// 3.3
	__read_line_values(in, city_pop);	// 3.4
	
	/// 4
	// 4.1
	in >> field;
	in >> eq;
	in >> n_centres;
	in >> semicolon;
	
	centres_wd.resize(n_centres);
	centres_cc.resize(n_centres);
	centres_ic.resize(n_centres);
	
	__read_line_values(in, centres_wd);	// 4.2
	__read_line_values(in, centres_cc);	// 4.3
	__read_line_values(in, centres_ic);	// 4.4
	
	// -- Finish reading file
	// move data from raw vectors into this object
	locations.resize(n_locations);
	for (size_t i = 0; i < n_locations; ++i) {
		locations[i].set(locations_x[i], locations_y[i]);
	}
	
	cities.resize(n_cities);
	for (size_t i = 0; i < n_cities; ++i) {
		cities[i].get_position().set(city_x[i], city_y[i]);
		cities[i].set_population(city_pop[i]);
	}
	
	centres.resize(n_centres);
	for (size_t i = 0; i < n_centres; ++i) {
		centres[i].set_working_distance(centres_wd[i]);
		centres[i].set_capacity(centres_cc[i]);
		centres[i].set_installation_cost(centres_ic[i]);
	}
	
	location_centre_type = vector<int>(n_locations, -1);
	cap_location = vector<double>(n_locations, 0.0);
	cit_by_prim = vector<int>(n_cities, -1);
	cit_by_sec = vector<int>(n_cities, -1);
	
	/// postprocess data
	
	// sort cities by population, store the ordering in sorted_cities
	sorted_cities.resize(n_cities);
	vector<pair<size_t, size_t> > cities_pop(n_cities);
	for (size_t c = 0; c < n_cities; ++c) {
		cities_pop[c].first = cities[c].get_population();
		cities_pop[c].second = c;
	}
	sort(cities_pop.begin(), cities_pop.end());
	for (size_t c = 0; c < n_cities; ++c) {
		sorted_cities[c] = cities_pop[c].second;
	}
	
	// sort centre types by installation cost, store the ordering in
	// sorted_centres
	sorted_centres.resize(n_centres);
	vector<pair<double, size_t> > centres_price(n_centres);
	for (size_t c = 0; c < n_centres; ++c) {
		centres_price[c].first = centres[c].get_installation_cost();
		centres_price[c].second = c;
	}
	sort(centres_price.begin(), centres_price.end());
	for (size_t c = 0; c < n_centres; ++c) {
		sorted_centres[c] = centres_price[c].second;
	}
}

void interface::print_input_data(ostream& os) {
	os << "D= " << D << endl;
	os << endl;
	
	os << "Locations: " << locations.size() << endl;
	for (size_t i = 0; i < locations.size(); ++i) {
		os << "    Location " << i << ":" << endl;
		os << "        Position: (" << locations[i].get_x() << ", "
		   << locations[i].get_y() << ")" << endl;
	}
	os << endl;
	
	os << "Cities: " << cities.size() << endl;
	for (size_t i = 0; i < cities.size(); ++i) {
		os << "    City " << i << ":" << endl;
		os << "        Position: (" << cities[i].get_position().get_x() << ", "
		   << cities[i].get_position().get_y() << ")" << endl;
		   
		os << "        Population: " << cities[i].get_population() << endl;
		os << endl;
	}
	
	os << "Centre types: " << centres.size() << endl;
	for (size_t i = 0; i < centres.size(); ++i) {
		os << "    Type " << i << ":" << endl;
		os << "        Working distance: " << centres[i].get_working_distance() << endl;
		os << "        Centre capacity: " << centres[i].get_capacity() << endl;
		os << "        Installation cost: " << centres[i].get_installation_cost() << endl;
		os << endl;
	}
}

size_t interface::get_n_cities() const {
	return n_cities;
}

size_t interface::get_n_locations() const {
	return n_locations;
}

size_t interface::get_n_centre_types() const {
	return n_centres;
}

void interface::print(ostream& os, const string& tab) const {
	os << tab << "Installed centres:" << endl;
	double totalCost = 0.0;
	
	for (size_t l = 0; l < locations.size(); ++l) {
		if (location_centre_type[l] != -1) {
			size_t centre_idx = location_centre_type[l];
			double instal_cost = centres[ centre_idx ].get_installation_cost();
			
			os << tab << "    At location " << l << " a centre of type: " << centre_idx << ". "
			          << "Cost= " << instal_cost << ". "
			          << "Max capacity= " << centres[centre_idx].get_capacity() << ". "
			          << "Working distance= " << centres[centre_idx].get_working_distance() << "." << endl;
			os << tab << "    Location's occupation= " << cap_location[l] << endl;
			
			os << tab << "    This location serves:" << endl;
			os << tab << "    " << "    -> As primary:";
			for (size_t c = 0; c < n_cities; ++c) {
				if (cit_by_prim[c] == l) {
					os << " " << c;
				}
			}
			os << endl;
			os << tab << "    " << "    -> As secondary:";
			for (size_t c = 0; c < n_cities; ++c) {
				if (cit_by_sec[c] == l) {
					os << " " << c;
				}
			}
			os << endl << endl;
			
			totalCost += instal_cost;
		}
	}
	
	os << tab << "    * Total cost= " << totalCost << endl;
	os << endl;
	
	os << tab << "Cities:" << endl;
	for (size_t c = 0; c < cities.size(); ++c) {
		os << tab << "City " << c << " served by:" << endl;
		
		os << tab << "    - Primary location: ";
		if (cit_by_prim[c] != -1) {
			os << cit_by_prim[c] << ". Distance= "
			   << cities[c].get_position().distance( locations[cit_by_prim[c]] );
		}
		os << endl;
		
		os << tab << "    - Secondary location: ";
		if (cit_by_prim[c] != -1) {
			os << cit_by_sec[c] << ". Distance= "
			   << cities[c].get_position().distance( locations[cit_by_sec[c]] );
		}
		os << endl << endl;
	}
	os << endl;
}

