#include "centre_type.hpp"

// PRIVATE

// PUBLIC

centre_type::centre_type(double wd, double cc, double ic) {
	working_distance = wd;
	centre_capacity = cc;
	installation_cost = ic;
}

/* OPERATORS */

bool centre_type::operator< (const centre_type& ct) const {
	return installation_cost < ct.installation_cost;
}

bool centre_type::operator> (const centre_type& ct) const {
	return installation_cost > ct.installation_cost;
}

/* SETTERS */

void centre_type::set_working_distance(double wd) {
	working_distance = wd;
}

void centre_type::set_capacity(double cc) {
	centre_capacity = cc;
}

void centre_type::set_installation_cost(double ic) {
	installation_cost = ic;
}

/* GETTERS */

double centre_type::get_working_distance() const {
	return working_distance;
}

double centre_type::get_capacity() const {
	return centre_capacity;
}

double centre_type::get_installation_cost() const {
	return installation_cost;
}

