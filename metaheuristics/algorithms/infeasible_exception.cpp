#include <metaheuristics/algorithms/infeasible_exception.hpp>

namespace metaheuristics {
namespace structures {
	
infeasible_exception::infeasible_exception() {
}

infeasible_exception::infeasible_exception(const string& msg) {
	message = msg;
}

infeasible_exception::~infeasible_exception() throw() {
}

const char* infeasible_exception::what() const throw() {
	return message.c_str();
}
	
} // -- namespace structures
} // -- namespace metaheuristics

