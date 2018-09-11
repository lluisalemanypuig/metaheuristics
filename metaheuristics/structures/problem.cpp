#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace structures {

// PROTECTED

// PUBLIC

template<class G>
problem<G>::problem() {
	n_neighbours_explored = 0;
}

template<class G>
problem<G>::~problem() { }

template<class G>
size_t problem<G>::get_n_neighbours_explored() const {
	return n_neighbours_explored;
}

} // -- namespace structures
} // -- namespace metaheuristics

