#include <metaheuristics/random/random_generator.hpp>

namespace metaheuristics {
namespace random {

template<class G, typename cT>
crandom_generator<G,cT>::crandom_generator() : random_generator<G,cT>() {
	U = nullptr;
}

template<class G, typename cT>
crandom_generator<G,cT>::~crandom_generator() {
	if (U != nullptr) {
		delete U;
	}
}

/* Initialisers of the distributions */

template<class G, typename cT>
void crandom_generator<G,cT>::init_uniform(cT a, cT b) {
	if (U != nullptr) {
		delete U;
		U = nullptr;
	}
	U = new uniform_real_distribution<cT>(a, b);
}

template<class G, typename cT>
void crandom_generator<G,cT>::init_binomial(cT, double) {
	// do nothing
}

/* Get pseudo-random numbers */

template<class G, typename cT>
cT crandom_generator<G,cT>::get_uniform() {
	assert(U != nullptr);

	cT r = (*U)(random_generator<G,cT>::gen);
	return r;
}

template<class G, typename cT>
cT crandom_generator<G,cT>::get_binomial() {
	return 0.0;
}

template<class G, typename cT>
void crandom_generator<G,cT>::make_n_uniform(cT *u, size_t N) {
	for (size_t i = 0; i < N; ++i) {
		u[i] = this->get_uniform();
	}
}

} // -- namespace random
} // -- namespace metaheuristics

