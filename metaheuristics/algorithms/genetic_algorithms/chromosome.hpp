#pragma once

// C includes
#include <stdlib.h>
#include <string.h>

// C++ includes
#include <fstream>
#include <vector>
#include <limits>
using namespace std;

namespace metaheuristics {
namespace structures {

class chromosome {
	private:
		double *genes;
		size_t n_genes;
		
	public:
		chromosome();
		~chromosome();
		
		// MEMORY HANDLING
		
		void init();
		void clear();
		
		// OPERATORS
		
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
		
		chromosome& operator= (const chromosome& c);
		double& operator[] (size_t i);
		const double& operator[] (size_t i) const;
		
		// SETTERS
		
		void set_size(size_t Ng);
		
		// GETTERS
		
		size_t size() const;
};

}
}

