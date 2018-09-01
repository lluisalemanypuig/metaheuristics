#include <metaheuristics/algorithms/metaheuristic.hpp>

namespace metaheuristics {
namespace algorithms {

metaheuristic::metaheuristic() {
}

metaheuristic::metaheuristic(random_number_generator *r) {
	rng = r;
}

metaheuristic::~metaheuristic() {
}

// MODIFIERS

void metaheuristic::set_rng(random_number_generator *_rng) {
	rng = _rng;
}

} // -- namespace algorithms
} // -- namespace metaheuristics

