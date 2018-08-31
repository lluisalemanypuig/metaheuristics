#include "individual.hpp"

namespace metaheuristics {
namespace structures {

individual::individual() {
	fitness = 0.0;
}

individual::individual(size_t N) {
	fitness = 0.0;
	c.set_size(N);
}

individual::~individual() { }

// MEMORY HANDLING
		
void individual::init() {
	c.init();
}

void individual::clear() {
	c.clear();
}

// SETTERS

void individual::set_fitness(double f) {
	fitness = f;
}

void individual::set_gene(size_t i, double g) {
	c[i] = g;
}

// GETTERS

chromosome& individual::get_chromosome() {
	return c;
}

const chromosome& individual::get_chromosome() const {
	return c;
}

double individual::get_fitness() const {
	return fitness;
}

double individual::get_gene(size_t i) const {
	return c[i];
}

}
}

