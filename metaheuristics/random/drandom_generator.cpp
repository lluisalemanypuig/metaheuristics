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

template<class G, typename dT>
drandom_generator<G,dT>::drandom_generator() : random_generator<G,dT>() {
	U = nullptr;
	B = nullptr;
}

template<class G, typename dT>
drandom_generator<G,dT>::~drandom_generator() {
	if (U != nullptr) {
		delete U;
	}
	if (B != nullptr) {
		delete B;
	}
}

/* Initialisers of the distributions */

template<class G, typename dT>
void drandom_generator<G,dT>::init_uniform(dT a, dT b) {
	if (U != nullptr) {
		delete U;
		U = nullptr;
	}
	U = new std::uniform_int_distribution<dT>(a, b);
}

template<class G, typename dT>
void drandom_generator<G,dT>::init_binomial(dT a, double p) {
	if (B != nullptr) {
		delete B;
		B = nullptr;
	}
	B = new std::binomial_distribution<dT>(a, p);
}

/* Get pseudo-random numbers */

template<class G, typename dT>
dT drandom_generator<G,dT>::get_uniform() {
	assert(U != nullptr);

	dT r = (*U)(random_generator<G,dT>::gen);
	return r;
}

template<class G, typename dT>
dT drandom_generator<G,dT>::get_binomial() {
	assert(B != nullptr);

	dT r = (*B)(random_generator<G,dT>::gen);
	return r;
}

template<class G, typename dT>
void drandom_generator<G,dT>::make_n_uniform(dT *u, size_t N) {
	for (size_t i = 0; i < N; ++i) {
		u[i] = this->get_uniform();
	}
}

} // -- namespace random
} // -- namespace metaheuristics

