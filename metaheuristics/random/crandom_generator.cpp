/*********************************************************************
 *
 * Collection of heuristic and metaheuristic algorithms
 *
 * Copyright (C) 2018-2020  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 *********************************************************************/

#pragma once

#include <metaheuristics/random/random_generator.hpp>

// C++ includes
#include <cassert>

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
	U = new std::uniform_real_distribution<cT>(a, b);
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

