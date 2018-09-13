#pragma once

namespace metaheuristics {
namespace structures {


/**
 * @brief Local Search Policy.
 * 
 * Policies when finding better solutions in the neighbourhood of
 * a problem:
 * - First Improvement: stop searching when a better solution than
 * the current has been found.
 * - Best Improvement: search all the neighbourhood and keep the best.
 */
enum local_search_policy {
	First_Improvement = false,
	Best_Improvement = true
};

} // -- namespace structures
} // -- namespace metaheuristics
