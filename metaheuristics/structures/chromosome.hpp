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

// C++ includes
#include <fstream>

namespace metaheuristics {
namespace structures {

/**
 * @brief Definition of a chromosome
 * 
 * A chromosome is a number of genes (see @ref n_genes)
 * that are basically floating point numbers between 0 and 1
 * (see @ref genes).
 */
class chromosome {
	public:
		/// Default constructor
		chromosome() = default;
		/// Destructor
		~chromosome();
		
		// MEMORY HANDLING
		
		/**
		 * @brief Allocates as many floating point values as @ref n_genes
		 * @pre Variable @ref n_genes must have a strictly positive value.
		 */
		void init();
		/**
		 * @brief Deallocates the memory occupied by @ref genes
		 * 
		 * Sets to 0 the number of genes (see @ref n_genes).
		 */
		void clear();
		
		// OPERATORS
		
		/// Operator <<
		inline friend
		std::ostream& operator<< (std::ostream& os, const chromosome& c) {
			os << "{";
			if (c.n_genes > 0) {
				os << c.genes[0];
				for (size_t g = 1; g < c.n_genes; ++g) {
					os << " " << c.genes[g];
				}
			}
			os << "}";
			return os;
		}
		
		/**
		 * @brief Assignation operator
		 * 
		 * Copies the values in @ref genes of @e c into this chromosome's
		 * genes.
		 * 
		 * The memory occupied by @ref genes is only freed and reallocated
		 * if the number of genes of this chromosome and @e c's differ.
		 * 
		 * @pre The size of chromosome @e c must be larger than 0.
		 */
		chromosome& operator= (const chromosome& c);
		/**
		 * @brief Acces the i-th gene
		 * 
		 * Returns a reference to the i-th gene
		 */
		double& operator[] (size_t i);
		/**
		 * @brief Acces the i-th gene
		 * 
		 * Returns a constant reference to the i-th gene
		 */
		const double& operator[] (size_t i) const;
		
		// SETTERS
		
		/**
		 * @brief Sets the number of genes of this chromosome
		 * 
		 * Sets the value of @ref n_genes to @e Ng
		 */
		void set_size(size_t Ng);
		
		// GETTERS
		
		/**
		 * @brief Returns the number of genes of this chromosome
		 * @returns Returns the value of @ref n_genes
		 */
		size_t size() const;

	private:
		/// The list of values between 0 and 1
		double *genes = nullptr;
		/// The size of @ref genes
		size_t n_genes = 0;
};

} // -- namespace structures
} // -- namespace metaheuristics

