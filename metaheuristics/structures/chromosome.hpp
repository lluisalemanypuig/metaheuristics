#pragma once

// C includes
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// C++ includes
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

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
	private:
		/// The list of values between 0 and 1
		double *genes;
		/// The size of @ref genes
		size_t n_genes;
		
	public:
		/// Default constructor
		chromosome();
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
		ostream& operator<< (ostream& os, const chromosome& c) {
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
};

} // -- namespace structures
} // -- namespace metaheuristics

