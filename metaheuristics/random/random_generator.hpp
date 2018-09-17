#pragma once

// C includes
#include <assert.h>

// C++ includes
#include <random>
using namespace std;

namespace metaheuristics {
namespace random {

/**
 * @brief Abstract pseudo-Random Number Generator (ARNG).
 *
 * Interface for random number generators using the C++11 header \<random\>.
 *
 * Seed the random engine calling method @ref seed_random_engine().
 *
 * Initialise whatever distribution is needed with the appropriate
 * method:
 * - @ref init_uniform(T a, T b) to generate numbers following
 *	the distribution U[a,b], where U denotes the uniform distribution.
 * - @ref init_binomial(T n, double p) to generate numbers following
 *	the distribution B(n,p), where B denotes the binomial distribution.
 *
 * @param G The random engine used to generate the numbers
 * @param T The type of the numbers generated (its type depends on the subclass
 * that will implement the interface)
 */
template<
	class G,	// class used for the random engine
	typename T	// type of the distribution
				// int, double, unsigned int, size_t, ...
>
class random_generator {
	protected:
		/// Random engine.
		G gen;
		
	public:
		/// Class constructor.
		random_generator();

		/// Class destructor.
		virtual ~random_generator();
		
		/// Initialises the random engine.
		virtual void seed_random_engine();
		
		/**
		 * @brief Initialise the uniform distribution.
		 * @param a Lower bound of the interval of the distribution.
		 * @param b Upper bound of the interval of the distribution.
		 */
		virtual void init_uniform(T a, T b) = 0;

		/**
		 * @brief Initialise the binomial distribution.
		 * @param n Number of independent experiments of the distribution.
		 * @param p Probability of success of each experiment.
		 */
		virtual void init_binomial(T n, double p) = 0;

		/// Compute a pseudo-random number uniformly at random.
		virtual T get_uniform() = 0;

		/// Compute a pseudo-random binomal number.
		virtual T get_binomial() = 0;
		
		/// Generate @e N uniform values and stores them in @e u
		virtual void make_n_uniform(T *u, size_t N) = 0;

}; // -- class random_generator

/**
 * @brief Discrete pseudo-Random Number Generator (DRNG).
 *
 * Class that generates discrete numbers in both uniform and binomial
 * distributions.
 *
 * @param G The random engine used to generate the numbers.
 * @param T The type of the numbers generated (int, char, unsigned int, ...).
 */
template<
	class G = default_random_engine,	// class used for the random engine
	typename dT = size_t				// type of the discrete distributions
										// int, char, unsigned int, size_t, ...
>
class drandom_generator : public random_generator<G,dT> {
	private:
		/// Object to generate integer numbers uniformly at random.
		uniform_int_distribution<dT> *U;

		/// Object to generate the numbers following a binomial distribution.
		binomial_distribution<dT> *B;
		
	public:
		/// Constructor.
		drandom_generator();
		/// Destructor.
		~drandom_generator();

		void init_uniform(dT a, dT b);
		void init_binomial(dT a, double p);

		dT get_uniform();
		dT get_binomial();
		
		void make_n_uniform(dT *u, size_t N);
		
}; // -- class drandom_generator

/**
 * @brief Continuous pseudo-Random Number Generator (CRNG).
 *
 * Class that generates continuous numbers only in the uniform distribution.
 *
 * @param G The random engine used to generate the numbers.
 * @param T The type of the numbers generated (double, float).
 */
template<
	class G = default_random_engine,	// class used for the random engine
	typename cT = double				// type of the continuous distributions
										// float, double
>
class crandom_generator : public random_generator<G,cT> {
	private:
		/// Object to generate floating point numbers uniformly at random.
		uniform_real_distribution<cT> *U;
		
	public:
		/// Constructor.
		crandom_generator();
		/// Destructor.
		~crandom_generator();

		void init_uniform(cT a, cT b);

		/// The CRNG leaves this method's body empty.
		void init_binomial(cT a, double p);

		cT get_uniform();

		/// In the CRNG this method always returns 0.
		cT get_binomial();
		
		void make_n_uniform(cT *u, size_t N);
		
}; // -- class crandom_generator

} // -- namespace random
} // -- namespace metaheuristics

#include <metaheuristics/random/random_generator.cpp>
#include <metaheuristics/random/drandom_generator.cpp>
#include <metaheuristics/random/crandom_generator.cpp>
