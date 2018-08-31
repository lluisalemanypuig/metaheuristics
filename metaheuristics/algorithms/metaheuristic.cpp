#include "metaheuristic.hpp"

namespace metaheuristics {
namespace algorithms {

metaheuristic::metaheuristic() {
}

metaheuristic::metaheuristic(random_number_generator *r) {
	rng = r;
}

metaheuristic::~metaheuristic() {
}

}
}
