#include <metaheuristics/random/random_generator.hpp>

namespace metaheuristics {
namespace random {

template<class G, typename T>
random_generator<G,T>::random_generator() { }

template<class G, typename T>
random_generator<G,T>::~random_generator() { }

template<class G, typename T>
void random_generator<G,T>::seed_random_engine() {
	
	// use a non-deterministic random number to seed the random engine
	random_device r;
	gen = G(r());
}

} // -- namespace random
} // -- namespace metaheuristics

