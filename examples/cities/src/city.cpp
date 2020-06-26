#include "city.hpp"

/* PRIVATE */

/* PUBLIC */

city::city(const point& l, size_t p) : location(l), population(p) {
}

/* OPERATORS */

bool city::operator< (const city& c) const {
	return population < c.population;
}

bool city::operator> (const city& c) const {
	return population > c.population;
}

/* SETTERS */

void city::set_position(const point& l) {
	location = l;
}

void city::set_population(size_t p) {
	population = p;
}

/* GETTERS */

point& city::get_position() {
	return location;
}

const point& city::get_position() const {
	return location;
}

size_t city::get_population() const {
	return population;
}

