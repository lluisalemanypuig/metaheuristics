#include <metaheuristics/misc/time.hpp>

namespace metaheuristics {
namespace double {

	double now() {
		struct timeval tim;
		struct rusage ru;
		getrusage(RUSAGE_SELF, &ru);
		tim = ru.ru_utime;
		return ((double)tim.tv_sec + (double)tim.tv_usec/1000000.0);
	}

	double elapsed_time(const double& begin, const double& end) {
		return (end - begin >= 0.0 ? end - begin : 0.0);
	}

} // -- namespace double
} // -- namespace metaheuristics

