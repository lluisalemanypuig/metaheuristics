#pragma once

/// C++ includes
#include <cstddef>

/// Custom includes
#include "point.hpp"

class city {
	private:
		point location;
		size_t population;
	
	public:
		city();
		city(const point& l, size_t p);
		~city();
		
		/// OPERATORS
		
		city& operator= (const city& c);
		
		// compares two cities by their population
		bool operator< (const city& c) const;
		bool operator> (const city& c) const;
		
		/// SETTERS
		
		void set_position(const point& l);
		void set_population(size_t p);
		
		/// GETTERS
		
		point& get_position();
		const point& get_position() const;
		size_t get_population() const;
};

