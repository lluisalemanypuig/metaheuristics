#include <metaheuristics/algorithms/metaheuristic.hpp>

namespace metaheuristics {
namespace algorithms {

template<class G, typename dT, typename cT>
metaheuristic<G,dT,cT>::metaheuristic() {
	drng = nullptr;
	crng = nullptr;
}

template<class G, typename dT, typename cT>
metaheuristic<G,dT,cT>::metaheuristic(drandom_generator<G,dT> *_drng, crandom_generator<G,cT> *_crng) {
	drng = _drng;
	crng = _crng;
}

template<class G, typename dT, typename cT>
metaheuristic<G,dT,cT>::~metaheuristic() { }

// MODIFIERS

template<class G, typename dT, typename cT>
void metaheuristic<G,dT,cT>::set_drng(drandom_generator<G,dT> *rng) {
	drng = rng;
}

template<class G, typename dT, typename cT>
void metaheuristic<G,dT,cT>::set_crng(crandom_generator<G,cT> *rng) {
	crng = rng;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

