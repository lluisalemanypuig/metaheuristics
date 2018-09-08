#include <metaheuristics/algorithms/problem.hpp>

namespace metaheuristics {
namespace structures {

// PROTECTED

// PUBLIC

problem<G,dT>::problem() {
	n_neighbours_explored = 0;
}

problem<G,dT>::~problem() { }

size_t problem<G,dT>::get_n_neighbours_explored() const {
	return n_neighbours_explored;
}

} // -- namespace structures
} // -- namespace metaheuristics

