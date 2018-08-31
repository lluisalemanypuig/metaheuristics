#include "chromosome.hpp"

namespace metaheuristics {
namespace structures {
	
chromosome::chromosome() {
	n_genes = 0;
	genes = NULL;
}

chromosome::~chromosome() {
	clear();
}

// MEMORY HANDLING

void chromosome::init() {
	if (n_genes > 0) {
		genes = (double *)malloc(n_genes*sizeof(double));
	}
}

void chromosome::clear() {
	if (genes != NULL) {
		n_genes = 0;
		free(genes);
		genes = NULL;
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
	return genes[i];
}

const double& chromosome::operator[] (size_t i) const {
	return genes[i];
}

// SETTERS

void chromosome::set_size(size_t Ng) {
	n_genes = Ng;
}

// GETTERS

size_t chromosome::size() const {
	return (genes != NULL ? n_genes : 0);
}
	
}
}

