/*********************************************************************
 * Collection of heuristic and metaheuristic algorithms
 * Copyright (C) 2018  Lluís Alemany Puig
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Contact: Lluís Alemany Puig (lluis.alemany.puig@gmail.com)
 * 
 *********************************************************************/

#pragma once

// C++ includes
#include <exception>
#include <string>
using namespace std;

namespace metaheuristics {
namespace structures {
 
/**
 * @brief Infeasibility cxception
 * 
 * This exception is used in those cases in which a solution
 * is infeasible.
 */
class infeasible_exception : public exception {
	protected:
		/// The message of the exception.
		string message;
		
	public:
		/// Default constructor.
		infeasible_exception();
		/// Constructor with message.
		infeasible_exception(const string& msg);
		/// Destructor.
		~infeasible_exception() throw();
		
		/// Returns the message of the exception.
		virtual const char* what() const throw();
};

} // -- namespace structures
} // -- namespace metaheuristics

