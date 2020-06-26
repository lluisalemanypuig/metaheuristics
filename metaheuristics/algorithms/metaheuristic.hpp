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

// metaheuristic includes
#include <metaheuristics/structures/problem.hpp>

namespace metaheuristics {
namespace algorithms {

/**
 * @brief Interface for the two functions common to all algorithms.
 * 
 * Common variables are:
 * - @ref seed_rng : whether the random generators should be seeded or not.
 * 
 * The methods to implement for are:
 * - @ref reset_algorithm : sets the algorithm to its initial state.
 * - @ref execute_algorithm.
 * 
 * @param G The type of engine used by the random number generators.
 */
template<
	class G = std::default_random_engine
>
class metaheuristic {
	public:
		/**
		 * @brief Default constructor.
		 * 
		 * Variable @ref seed_rng is initialised to @e false.
		 */
		metaheuristic() = default;
		/// Destructor.
		virtual ~metaheuristic() = default;
		
		// MODIFIERS
		
		/**
		 * @brief Seed the random number generators.
		 * 
		 * Sets variable @ref seed_rng to true.
		 */
		void seed();
		
		/**
		 * @brief Sets the algorithm to its original state.
		 * 
		 * For example, if the algorithm has variables storing the
		 * number of iterations, execution time, ... here is where
		 * one would set all these variables to 0.
		 */
		virtual void reset_algorithm() = 0;
		
		/**
		 * @brief Starts the execution of the algorithm.
		 * @returns Returns true if it terminates succesfully. Returns
		 * false if otherwise.
		 */
		virtual bool execute_algorithm(structures::problem<G> *p, double& c) = 0;

	protected:
		/// Variable telling whether the number generators should be seeded or not.
		bool seed_rng = false;
};

/// Shorthand for the genetic_algorithm class template
template<class G>
using META = metaheuristic<G>;

} // -- namespace algorithms
} // -- namespace metaheuristics

#include <metaheuristics/algorithms/metaheuristic.cpp>
