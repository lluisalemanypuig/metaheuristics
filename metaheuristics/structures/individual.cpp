/*********************************************************************
 *
 * Collection of heuristic and metaheuristic algorithms
 *
 * Copyright (C) 2018-2020  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 *********************************************************************/

#include <metaheuristics/structures/individual.hpp>

namespace metaheuristics {
namespace structures {

individual::individual(size_t n) {
	fitness = 0.0;
	c.set_size(n);
}

// MEMORY HANDLING
		
void individual::init() {
	fitness = 0;
	c.init();
}

void individual::clear() {
	fitness = 0;
	c.clear();
}

// OPERATRS

individual& individual::operator= (const individual& i) {
	fitness = i.fitness;
	c = i.c;
	return *this;
}

// SETTERS

void individual::set_fitness(double f) {
	fitness = f;
}

void individual::set_gene(size_t i, double g) {
	c[i] = g;
}

void individual::set_n_genes(size_t n) {
	c.set_size(n);
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

} // -- namespace structures
} // -- namespace metaheuristics

