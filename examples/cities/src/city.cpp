#include "city.hpp"

/// PRIVATE

/// PUBLIC

city::city() {
}

city::city(const point& l, size_t p) : location(l), population(p) {
}

city::~city() {
}

/// OPERATORS

city& city::operator= (const city& c) {
	location = c.location;
	population = c.population;
	return *this;
}

bool city::operator< (const city& c) const {
	return population < c.population;
}

bool city::operator> (const city& c) const {
	return population > c.population;
}

/// SETTERS

void city::set_position(const point& l) {
	location = l;
}

void city::set_population(size_t p) {
	population = p;
}

/// GETTERS

point& city::get_position() {
	return location;
}

const point& city::get_position() const {
	return location;
}

size_t city::get_population() const {
	return population;
}

