#include <metaheuristics/algorithms/problems>

namespace metaheuristics {
namespace structures {

// PROTECTED

// PUBLIC

problem::problem() {
	n_neighbours_explored = 0;
}

problem::~problem() { }

size_t problem::get_n_neighbours_explored() const {
	return n_neighbours_explored;
}

} // -- namespace structures
} // -- namespace metaheuristics

