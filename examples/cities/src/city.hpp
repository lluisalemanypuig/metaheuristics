#pragma once

// C++ includes
#include <cstddef>

// Custom includes
#include "point.hpp"

class city {
	public:
		city() = default;
		city(const point& l, int p);
		~city() = default;
		
		// OPERATORS

		// compares two cities by their population
		bool operator< (const city& c) const;
		bool operator> (const city& c) const;
		
		// SETTERS
		
		void set_position(const point& l);
		void set_population(int p);
		
		// GETTERS
		
		point& get_position();
		const point& get_position() const;
		int get_population() const;

	private:
		point location;
		int population;
};

