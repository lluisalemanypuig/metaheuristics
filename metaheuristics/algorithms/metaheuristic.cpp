#include <metaheuristics/algorithms/metaheuristic.hpp>

namespace metaheuristics {
namespace algorithms {

metaheuristic::metaheuristic() {
}

metaheuristic::metaheuristic(random_generator<> *r) {
	rng = r;
}

metaheuristic::~metaheuristic() {
}

// MODIFIERS

void metaheuristic::set_rng(random_generator<> *_rng) {
	rng = _rng;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

