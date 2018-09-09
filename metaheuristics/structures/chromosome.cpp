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
	assert(n_genes > 0);
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
	assert(c.n_genes > 0);
	
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

