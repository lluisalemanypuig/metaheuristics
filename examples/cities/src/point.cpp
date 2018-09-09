#include "point.hpp"

/// PRIVATE

/// PUBLIC

point::point() {
}

point::point(double _x, double _y) : x(_x), y(_y) {
}

point::point(const point& p) {
	x = p.x;
	y = p.y;
}

point::~point() {
}

/// OPERATORS

point& point::operator= (const point& p) {
	x = p.x;
	y = p.y;
	return *this;
}

/// SETTERS

void point::set(double _x, double _y) {
	x = _x;
	y = _y;
}

void point::set_x(double _x) {
	x = _x;
}

void point::set_y(double _y) {
	y = _y;
}

/// GETTERS

double point::get_x() const {
	return x;
}

double point::get_y() const {
	return y;
}

double point::distance(const point& p) const {
	return sqrt(distance2(p));
}

double point::distance2(const point& p) const {
	double dx = x - p.x;
	double dy = y - p.y;
	return dx*dx + dy*dy;
}

