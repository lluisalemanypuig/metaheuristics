#pragma once

// C++ includes
#include <fstream>
#include <cmath>
using namespace std;

class point {
	public:
		point() = default;
		point(double x, double y);
		point(const point& p);
		~point() = default;
		
		/* OPERATORS */
		
		inline friend
		ostream& operator<< (ostream& os, const point& p) {
			os << "(" << p.x << ", " << p.y << ")";
			return os;
		}
		
		point& operator= (const point& p);
		
		/* SETTERS */
		
		void set(double x, double y);
		void set_x(double x);
		void set_y(double y);
		
		/* GETTERS */
		
		double get_x() const;
		double get_y() const;
		
		double distance(const point& p) const;
		double distance2(const point& p) const;
		
		static inline
		double distance(const point& p, const point& q) {
			return p.distance(q);
		}
		
		static inline
		double distance2(const point& p, const point& q) {
			return p.distance2(q);
		}

	private:
		double x = 0.0;
		double y = 0.0;
};

