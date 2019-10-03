/*********************************************************************
 * Collection of heuristic and metaheuristic algorithms
 * Copyright (C) 2018  Lluís Alemany Puig
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

#include <metaheuristics/structures/chromosome.hpp>

namespace metaheuristics {
namespace structures {
	
chromosome::chromosome() {
	n_genes = 0;
	genes = nullptr;
}

chromosome::~chromosome() {
	clear();
}

// MEMORY HANDLING

void chromosome::init() {
	assert(0 < n_genes);
	
	genes = (double *)malloc(n_genes*sizeof(double));
}

void chromosome::clear() {
	if (genes != nullptr) {
		n_genes = 0;
		free(genes);
		genes = nullptr;
	}
}

// OPERATORS

chromosome& chromosome::operator= (const chromosome& c) {
	if (n_genes != c.n_genes) {
		clear();
		n_genes = c.n_genes;
		genes = (double *)malloc(n_genes*sizeof(double));
	}
	
	memcpy(genes, c.genes, n_genes*sizeof(double));
	
	return *this;
}

double& chromosome::operator[] (size_t i) {
	assert(i < n_genes);
	return genes[i];
}

const double& chromosome::operator[] (size_t i) const {
	assert(i < n_genes);
	return genes[i];
}

// SETTERS

void chromosome::set_size(size_t Ng) {
	n_genes = Ng;
}

// GETTERS

size_t chromosome::size() const {
	return (genes != nullptr ? n_genes : 0);
}
	
} // -- namespace structures
} // -- namespace metaheuristics

