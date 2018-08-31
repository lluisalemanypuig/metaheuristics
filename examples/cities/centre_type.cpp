#include "centre_type.hpp"

/// PRIVATE

/// PUBLIC

centre_type::centre_type() {
	working_distance = 0.0;
	centre_capacity = 0;
	installation_cost = 0.0;
}

centre_type::centre_type(double wd, size_t cc, double ic) {
	working_distance = wd;
	centre_capacity = cc;
	installation_cost = ic;
}

centre_type::~centre_type() { }

/// OPERATORS

centre_type& centre_type::operator= (const centre_type& ct) {
	working_distance = ct.working_distance;
	centre_capacity = ct.centre_capacity;
	installation_cost = ct.installation_cost;
	return *this;
}

bool centre_type::operator< (const centre_type& ct) const {
	return installation_cost < ct.installation_cost;
}

bool centre_type::operator> (const centre_type& ct) const {
	return installation_cost > ct.installation_cost;
}

/// SETTERS

void centre_type::set_working_distance(double wd) {
	working_distance = wd;
}

void centre_type::set_capacity(size_t cc) {
	centre_capacity = cc;
}

void centre_type::set_installation_cost(double ic) {
	installation_cost = ic;
}

/// GETTERS

double centre_type::get_working_distance() const {
	return working_distance;
}

size_t centre_type::get_capacity() const {
	return centre_capacity;
}

double centre_type::get_installation_cost() const {
	return installation_cost;
}

