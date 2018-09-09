#pragma once

/// C++ includes
#include <cstddef>

class centre_type {
	private:
		double working_distance;
		size_t centre_capacity;
		double installation_cost;
		
	public:
		centre_type();
		centre_type(double wd, size_t cc, double ic);
		~centre_type();
		
		/// OPERATORS
		
		centre_type& operator= (const centre_type& ct);
		
		// compares two centre types by their installation cost
		bool operator< (const centre_type& ct) const;
		bool operator> (const centre_type& ct) const;
		
		/// SETTERS
		
		void set_working_distance(double wd);
		void set_capacity(size_t cc);
		void set_installation_cost(double ic);
		
		/// GETTERS
		
		double get_working_distance() const;
		size_t get_capacity() const;
		double get_installation_cost() const;
};

