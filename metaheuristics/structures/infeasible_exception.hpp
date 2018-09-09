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

