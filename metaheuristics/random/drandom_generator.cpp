#include <metaheuristics/random/random_generator.hpp>

namespace metaheuristics {
namespace random {

template<class G, typename dT>
drandom_generator<G,dT>::drandom_generator() : random_generator<G,dT>() { }

template<class G, typename dT>
drandom_generator<G,dT>::~drandom_generator() { }

/* Initialisers of the distributions */

template<class G, typename dT>
void drandom_generator<G,dT>::init_uniform(dT a, dT b) {
	U = uniform_int_distribution<dT>(a, b);
}

template<class G, typename dT>
void drandom_generator<G,dT>::init_binomial(dT a, double p) {
	B = binomial_distribution<dT>(a, p);
}

/* Get pseudo-random numbers */

template<class G, typename dT>
dT drandom_generator<G,dT>::get_uniform() {
	dT r = U(random_generator<G,dT>::gen);
	return r;
}

template<class G, typename dT>
dT drandom_generator<G,dT>::get_binomial() {
	dT r = B(random_generator<G,dT>::gen);
	return r;
}
	
} // -- namespace random
} // -- namespace metaheuristics

