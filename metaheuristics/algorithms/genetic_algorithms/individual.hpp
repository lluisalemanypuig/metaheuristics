#pragma once

// C++ includes
#include <fstream>
using namespace std;

// Custom includes
#include "algorithms/genetic_algorithms/chromosome.hpp"

namespace metaheuristics {
namespace structures {

template<class T>
class allocator_individual {
	public:
		// typedefs
		typedef T						value_type;
		typedef value_type* 			pointer;
		typedef const value_type* 	const_pointer;
		typedef value_type& 			reference;
		typedef const value_type&	const_reference;
		typedef size_t 				size_type;
		typedef ptrdiff_t	 			difference_type;
		
		template<typename U>
		struct rebind {
			typedef allocator_individual<U> other;
		};
		
		inline explicit allocator_individual() { }
		inline ~allocator_individual() { }
		inline explicit allocator_individual(const allocator_individual&) { }
		
		template<typename U>
		inline explicit allocator_individual(const allocator_individual<U>&) { }
		
		// address
		inline pointer address(reference r) {
			return &r;
		}
		
		inline const_pointer address(const_reference r) {
			return &r;
		}
		
		// memory allocation
		inline pointer allocate
		(
			size_type cnt,
			typename std::allocator<void>::const_pointer = 0
		)
		{ 
			return reinterpret_cast<pointer>(::operator new(cnt*sizeof(T))); 
		}
		
		inline void deallocate(pointer p, size_type) {
			::operator delete(p);
		}
		
		// size
		inline size_type max_size() const { 
			//return std::numeric_limits<size_type>::max()/sizeof(T);
			return 0;
		}
		
		// construction
		inline void construct(pointer p, const T& t) {
			new(p) T(t);
			p->init();
		}
		
		// destruction
		inline void destroy(pointer p) {
			p->clear();
		}
		
		inline bool operator== (const allocator_individual&) { return true; }
		inline bool operator!= (const allocator_individual& a) { return not operator==(a); }
};

class individual {
	private:
		chromosome c;
		double fitness;
	
	public:
		individual();
		individual(size_t N);
		~individual();
		
		// MEMORY HANDLING
		
		void init();
		void clear();
		
		// OPERATORS
		
		inline friend
		ostream& operator<< (ostream& os, const individual& i) {
			os << "fitness= " << i.fitness << ", chromosome= " << i.c;
			return os;
		}
		
		// SETTERS
		
		void set_fitness(double f);
		void set_gene(size_t i, double g);
		
		// GETTERS
		
		chromosome& get_chromosome();
		const chromosome& get_chromosome() const;
		
		double get_fitness() const;
		double get_gene(size_t i) const;
};

}
}

