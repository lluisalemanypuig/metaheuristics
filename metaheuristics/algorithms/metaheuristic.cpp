#include <metaheuristics/algorithms/metaheuristic.hpp>

namespace metaheuristics {
namespace algorithms {

template<class G, typename dT>
metaheuristic<G,dT>::metaheuristic() {
	seed_rng = false;
}

template<class G, typename dT>
metaheuristic<G,dT>::~metaheuristic() { }

// MODIFIERS

template<class G, typename dT>
void metaheuristic<G,dT>::seed() {
	seed_rng = true;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

