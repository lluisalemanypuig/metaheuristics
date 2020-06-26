#pragma once

class centre_type {
	public:
		centre_type() = default;
		centre_type(double wd, double cc, double ic);
		~centre_type() = default;
		
		// OPERATORS

		// compares two centre types by their installation cost
		bool operator< (const centre_type& ct) const;
		bool operator> (const centre_type& ct) const;
		
		// SETTERS
		
		void set_working_distance(double wd);
		void set_capacity(double cc);
		void set_installation_cost(double ic);
		
		// GETTERS
		
		double get_working_distance() const;
		double get_capacity() const;
		double get_installation_cost() const;

	private:
		double working_distance = 0.0;
		double centre_capacity = 0.0;
		double installation_cost = 0.0;
};

