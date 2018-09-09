#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace structures {

// PROTECTED

// PUBLIC

template<class G, typename dT>
problem<G,dT>::problem() {
	n_neighbours_explored = 0;
}

template<class G, typename dT>
problem<G,dT>::~problem() { }

template<class G, typename dT>
size_t problem<G,dT>::get_n_neighbours_explored() const {
	return n_neighbours_explored;
}

} // -- namespace structures
} // -- namespace metaheuristics

