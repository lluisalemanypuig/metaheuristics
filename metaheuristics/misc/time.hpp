#pragma once

// C includes
#include <sys/resource.h>
#include <stdlib.h>

namespace metaheuristics {
namespace timing {

	/// Returns the current time (in seconds)
	double now();
	
	/// Returns the elapsed time between begin and end in seconds
	double elapsed_time(const double& begin, const double& end);

} // -- namespace timing
} // -- namespace metaheuristics

