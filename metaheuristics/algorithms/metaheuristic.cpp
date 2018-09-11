#include <metaheuristics/algorithms/metaheuristic.hpp>

namespace metaheuristics {
namespace algorithms {

template<class G>
metaheuristic<G>::metaheuristic() {
	seed_rng = false;
}

template<class G>
metaheuristic<G>::~metaheuristic() { }

// MODIFIERS

template<class G>
void metaheuristic<G>::seed() {
	seed_rng = true;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

