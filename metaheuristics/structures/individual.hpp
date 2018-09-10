#pragma once

// C++ includes
#include <fstream>
using namespace std;

// Custom includes
#include <metaheuristics/structures/chromosome.hpp>

namespace metaheuristics {
namespace structures {

/**
 * @brief Class defining allocator for the class @ref individual.
 * 
 * Used for faster deallocation of resources.
 */
template<class T>
class allocator_individual {
	public:
		// typedefs
		typedef T					value_type;
		typedef value_type* 		pointer;
		typedef const value_type* 	const_pointer;
		typedef value_type& 		reference;
		typedef const value_type&	const_reference;
		typedef size_t 				size_type;
		typedef ptrdiff_t	 		difference_type;
		
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
			return std::numeric_limits<size_type>::max()/sizeof(T);
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

/**
 * @brief Definition of individual.
 * 
 * An individual (in the context of genetic algorithms) is a chromosome
 * (see @ref c) and its value of fitness (see @ref fitness).
 */
class individual {
	private:
		/// The chromosome of the individual.
		chromosome c;
		/// The fitness of the individual.
		double fitness;
	
	public:
		/// Default constructor.
		individual();
		/**
		 * @brief Constructor with number of genes.
		 * @param n Number of genes of chromosome @ref c.
		 */
		individual(size_t n);
		/// Destructor.
		~individual();
		
		// MEMORY HANDLING
		
		/**
		 * @brief Initialises the individual.
		 * 
		 * Allocates the necessary for the chromosome.
		 * Sets @ref fitness to 0.
		 * @pre The same preconditions as @ref chromosome::init().
		 */
		void init();
		/**
		 * @brief Clears the chromosome.
		 * 
		 * Deallocates the memory occupied by the chromosome and
		 * sets @ref fitness to 0.
		 */
		void clear();
		
		// OPERATORS
		
		/// Operator <<.
		inline friend
		ostream& operator<< (ostream& os, const individual& i) {
			os << "fitness= " << i.fitness << ", chromosome= " << i.c;
			return os;
		}
		/// Operator =.
		individual& operator= (const individual& i);
		
		// SETTERS
		
		/**
		 * @brief Sets the fitness of this individual.
		 * 
		 * Sets the value of @ref fitness to @e f.
		 */
		void set_fitness(double f);
		/**
		 * @brief Sets the value of the i-th gene of this individual.
		 * 
		 * Sets the i-th value of @ref chromosome::genes to @e g.
		 */
		void set_gene(size_t i, double g);
		/**
		 * @brief Sets the number of genes of this individual's chromosome.
		 * 
		 * Sets the value of @ref chromosome::n_genes to @e n.
		 */
		void set_n_genes(size_t n);
		
		// GETTERS
		
		/// Returns a non-constant refernce of @ref c.
		chromosome& get_chromosome();
		/// Returns a constant refernce of @ref c.
		const chromosome& get_chromosome() const;
		
		/**
		 * @brief Returns the fitness of this individual.
		 * @returns Returns the value of @ref fitness.
		 */
		double get_fitness() const;
		/**
		 * @brief Returns the i-th gene of this individual.
		 * @returns Returns the i-th value of @ref chromosome::genes.
		 */
		double get_gene(size_t i) const;
};

} // -- namespace structures
} // -- namespace metaheuristics

