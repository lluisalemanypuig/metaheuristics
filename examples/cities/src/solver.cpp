#include "solver.hpp"

/// PRIVATE

/// PUBLIC

solver::solver() : interface() {
}

solver::~solver() {
	
}

problem<> *solver::empty() const {
	solver *s = new solver();
	s->copy_empty(this);
	return s;
}

problem<> *solver::clone() const {
	solver *s = new solver();
	s->copy_interface(this);
	return s;
}

void solver::copy(const problem *p) {
	const solver *s = static_cast<const solver *>(p);
	this->copy_interface(s);
}

void solver::clear() {
	this->empty_interface();
}

