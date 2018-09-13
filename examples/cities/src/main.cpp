/// C includes
#include <string.h>

/// C++ includes
#include <iostream>
#include <fstream>
using namespace std;

/// metaheuristics includes
#include <metaheuristics/algorithms.hpp>
#include <metaheuristics/structures.hpp>

/// Custom includes
#include "solver.hpp"

using namespace metaheuristics;
using namespace structures;
using namespace algorithms;
using namespace random;

void print_usage() {
	cout << "Locations, Cities, and Centres" << endl;
	cout << "Usage:" << endl;
	cout << "    [-h, --help]:        shows this help" << endl;
	cout << "    [-s, --seed]:        seed the random number generators" << endl;
	cout << "    [-i, --input] f:     the input file with the description of the instance" << endl;
	cout << "    [--optimal] o:       value of the optimal solution (extracted from the ILP solution" << endl;
	cout << "    [-a, --algorithm] a: the algorithm to be executed. Possible values:" << endl;
	cout << "        - local-search:  constructs an initial solution deterministically" << endl;
	cout << "                         (i.e., not randomly) and, following a given policy, improves" << endl;
	cout << "                         the solution exploring the neighbourhood" << endl;
	cout << "        - grasp:         aplies the GRASP metaheuristic, using the parameter alpha" << endl;
	cout << "                         for the randomised choice of candidates" << endl;
	cout << "        - rkga:          aplies the RKGA metaheuristic" << endl;
	cout << "        - brkga:         aplies the BRKGA metaheuristic" << endl;
	cout << endl;
	
	cout << "Optional parametres:" << endl;
	cout << "* For the algorithms:" << endl;
	cout << "    -> Local search algorithm:" << endl;
	cout << "        [--iter-local] i:      maximum number of iterations for the local search algorithm. Default: 10" << endl;
	cout << "        [--policy] p:          policy that the algorithm will apply. Default: Best" << endl;
	cout << "            Possible values:" << endl;
	cout << "            - First:           First improvement" << endl;
	cout << "            - Best:            Best improvement" << endl;
	cout << "    -> GRASP:" << endl;
	cout << "        [--alpha] a:           randomness parameter. Default = 1" << endl;
	cout << "        [--iter-local] i:      maximum number of iterations for the local search algorithm. Default: 10" << endl;
	cout << "        [--iter-grasp] i:      maximum number of iterations for the GRASP metaheuristic. Default: 10" << endl;
	cout << "        [--policy] p:          policy that the algorithm will apply. Default: Best" << endl;
	cout << "            Possible values:" << endl;
	cout << "            - First:           First improvement" << endl;
	cout << "            - Best:            Best improvement" << endl;
	cout << "    -> RKGA:" << endl;
	cout << "        [--num-gen] i:         maximum number of generations. Default: 10" << endl;
	cout << "        [--pop-size] s:        size of the total population. Default: 0" << endl;
	cout << "        [--mut-size] s:        size of the mutant population. Default: 0" << endl;
	cout << "        [--inher-prob] p:      probability of inheritance. Default: 0.5" << endl;
	cout << "    -> BRKGA:" << endl;
	cout << "        [--num-gen] i:         maximum number of generations. Default: 10" << endl;
	cout << "        [--pop-size] s:        size of the total population. Default: 0" << endl;
	cout << "        [--mut-size] s:        size of the mutant population. Default: 0" << endl;
	cout << "        [--elite-size] s:      size of the elite population set. Default: 0" << endl;
	cout << "        [--inher-prob] p:      probability of inheritance. Default: 0.5" << endl;
	cout << endl;
}

local_search_policy parse_policy(int argc, char *argv[], int i) {
	local_search_policy lsp;
	if (strcmp(argv[i], "First") == 0) {
		lsp = First_Improvement;
	}
	else if (strcmp(argv[i], "Best") == 0) {
		lsp = Best_Improvement;
	}
	else {
		cerr << "Unknown value '" << string(argv[i]) << "' for local search policy" << endl;
	}
	return lsp;
}

class local_search_params {
	private:
	public:
		size_t MAX_ITERATIONS;
		local_search_policy POLICY;
		
		local_search_params() {
			MAX_ITERATIONS = 10;
			POLICY = Best_Improvement;
		}
		~local_search_params() {}
		
};

void parse_local_search_params(int argc, char *argv[], local_search_params& ls_params) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--iter-local") == 0) {
			ls_params.MAX_ITERATIONS = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--policy") == 0) {
			ls_params.POLICY = parse_policy(argc, argv, i + 1);
			++i;
		}
	}
}

class grasp_params {
	private:
	public:
		size_t MAX_IT_LOCAL;
		size_t MAX_IT_GRASP;
		local_search_policy POLICY;
		double ALPHA;
		
		grasp_params() {
			MAX_IT_GRASP = MAX_IT_LOCAL = 10;
			POLICY = Best_Improvement;
			ALPHA = 1.0;
		}
		~grasp_params() { }
};

void parse_grasp_params(int argc, char *argv[], grasp_params& params) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--iter-local") == 0) {
			params.MAX_IT_LOCAL = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--iter-grasp") == 0) {
			params.MAX_IT_GRASP = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--policy") == 0) {
			params.POLICY = parse_policy(argc, argv, i + 1);
			++i;
		}
		else if (strcmp(argv[i], "--alpha") == 0) {
			params.ALPHA = atof(argv[i + 1]);
			++i;
		}
	}
}

class rkga_params {
	private:
	public:
		size_t NUM_GENERATIONS;
		size_t POPULATION_SIZE;
		size_t MUTANT_POPULATION_SIZE;
		double INHER_PROB;
		
		rkga_params() {
			NUM_GENERATIONS = 10;
			POPULATION_SIZE = 0;
			MUTANT_POPULATION_SIZE = 0;
			INHER_PROB = 0.5;
		}
		~rkga_params() { }
};

void parse_rkga_params(int argc, char *argv[], rkga_params& params) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--num-gen") == 0) {
			params.NUM_GENERATIONS = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--pop-size") == 0) {
			params.POPULATION_SIZE = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--mut-size") == 0) {
			params.MUTANT_POPULATION_SIZE = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--inher-prob") == 0) {
			params.INHER_PROB = atof(argv[i + 1]);
			++i;
		}
	}
}

class brkga_params {
	private:
	public:
		size_t NUM_GENERATIONS;
		size_t POPULATION_SIZE;
		size_t MUTANT_POPULATION_SIZE;
		size_t ELITE_SET_SIZE;
		double INHER_PROB;
		
		brkga_params() {
			NUM_GENERATIONS = 10;
			POPULATION_SIZE = ELITE_SET_SIZE = 0;
			MUTANT_POPULATION_SIZE = 0;
			INHER_PROB = 0.5;
		}
		~brkga_params() { }
};

void parse_brkga_params(int argc, char *argv[], brkga_params& params) {
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "--num-gen") == 0) {
			params.NUM_GENERATIONS = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--pop-size") == 0) {
			params.POPULATION_SIZE = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--mut-size") == 0) {
			params.MUTANT_POPULATION_SIZE = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--elite-size") == 0) {
			params.ELITE_SET_SIZE = atoi(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--inher-prob") == 0) {
			params.INHER_PROB = atof(argv[i + 1]);
			++i;
		}
	}
}

int main(int argc, char *argv[]) {
	/* ******************************** */
	// ------- BASIC OUTPUT SETUP ----- //
	/* ******************************** */
	
	cout.setf(ios::fixed);
	cout.precision(4);
	
	/* ******************************** */
	// ------- PARSE PARAMATERS ------- //
	/* ******************************** */
	
	bool use_optimal_value = false;
	double optimal_value = 0.0;
	bool seed = false;
	string input_filename, algorithm;
	input_filename = algorithm = "none";
	
	for (int i = 1; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0 or strcmp(argv[i], "--help") == 0) {
			print_usage();
			return 0;
		}
		else if (strcmp(argv[i], "-s") == 0 or strcmp(argv[i], "--seed") == 0) {
			seed = true;
		}
		else if (strcmp(argv[i], "-i") == 0 or strcmp(argv[i], "--input") == 0) {
			input_filename = string(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "--optimal") == 0) {
			use_optimal_value = true;
			optimal_value = atof(argv[i + 1]);
			++i;
		}
		else if (strcmp(argv[i], "-a") == 0 or strcmp(argv[i], "--algorithm") == 0) {
			algorithm = string(argv[i + 1]);
			++i;
		}
	}
	
	if (algorithm == "none") {
		cerr << "Error: Missing value for algorithm. Use option [-a, --algorithm]" << endl;
		return 1;
	}
	
	if (input_filename == "none") {
		cerr << "Error: Missing input filename. Use option [-i, --input]" << endl;
		return 1;
	}
	
	if (algorithm != "local-search" and algorithm != "grasp" and algorithm != "rkga" and algorithm != "brkga") {
		cerr << "Error: Wrong value for algorithm parameter" << endl;
		return 1;
	}
	
	local_search_params ls_params;
	grasp_params gs_params;
	rkga_params r_params;
	brkga_params br_params;
	
	if (algorithm == "local-search") {
		parse_local_search_params(argc, argv, ls_params);
	}
	else if (algorithm == "grasp") {
		parse_grasp_params(argc, argv, gs_params);
	}
	else if (algorithm == "rkga") {
		parse_rkga_params(argc, argv, r_params);
	}
	else if (algorithm == "brkga") {
		parse_brkga_params(argc, argv, br_params);
	}
	
	/* ******************************** */
	// ------- READ INPUT DATA -------- //
	/* ******************************** */
	
	ifstream fin;
	fin.open(input_filename.c_str());
	if (!fin.is_open()) {
		cerr << "File " << input_filename << " does not exist" << endl;
		return 1;
	}
	
	bool sane;
	solver *s = new solver();
	s->read_from_file(fin);
	//s->print_input_data();
	
	/* ******************************** */
	// ------- EXECUTE ALGORITHM ------ //
	/* ******************************** */
	
	if (algorithm == "local-search") {
		local_search<> ls(ls_params.MAX_ITERATIONS, ls_params.POLICY);
		cout << "Local search:" << endl;
		
		try {
			double eval = s->greedy_construct();
			sane = s->sanity_check();
			cout << "    Is initial solution sane? " << (sane ? "Yes" : "No") << endl;
			
			if (seed) {
				ls.seed();
			}
			
			ls.execute_algorithm(s, eval);
			
			//cout << "    Local search solution:" << endl;
			//s->print("    ");
			
			sane = s->sanity_check();
			cout << "    Is final solution sane? " << (sane ? "Yes" : "No") << endl;
			cout << "    Final solution's cost: " << -eval << endl;
			if (use_optimal_value) {
				cout << "        Gap = LS - ILP = " << -eval << " - " << optimal_value << " = " << -eval - optimal_value << endl;
			}
		}
		catch (const infeasible_exception& e) {
			cerr << "main: Infeasible solution when greedily constructing "
				 << "an initial solution with the local search procedure." << endl;
			cerr << "Message:" << endl;
			cerr << e.what() << endl;
		}
	}
	else if (algorithm == "grasp") {
		cout << "GRASP:" << endl;
		
		grasp<> gs
		(
			gs_params.MAX_IT_GRASP,
			gs_params.MAX_IT_LOCAL,
			gs_params.ALPHA,
			gs_params.POLICY
		);
		
		if (seed) {
			gs.seed();
		}
		
		double eval;
		gs.execute_algorithm(s, eval);
		
		//cout << "    GRASP solution:" << endl;
		//s->print("    ");
		
		cout << endl;
		cout << endl;
		
		bool sane = s->sanity_check();
		cout << "    Is final solution sane? " << (sane ? "Yes" : "No") << endl;
		cout << "    Final solution's cost: " << -eval << endl;
		if (use_optimal_value) {
			cout << "        Gap = GRASP - ILP = " << -eval << " - " << optimal_value << " = " << -eval - optimal_value << endl;
		}
	}
	else if (algorithm == "rkga") {
		cout << "RKGA:" << endl;
		
		rkga<> r
		(
			r_params.POPULATION_SIZE,
			r_params.MUTANT_POPULATION_SIZE,
			r_params.NUM_GENERATIONS,
			s->get_n_cities(),
			r_params.INHER_PROB
		);
		
		if (seed) {
			r.seed();
		}
		
		double eval;
		r.execute_algorithm(s, eval);
		
		//cout << "    BRKGA solution:" << endl;
		//s->print("    ");
		
		bool sane = s->sanity_check();
		cout << endl << "    Is final solution sane? " << (sane ? "Yes" : "No") << endl;
		cout << "    Final solution's cost: " << -eval << endl;
		if (use_optimal_value) {
			cout << "        Gap = RKGA - ILP = " << -eval << " - " << optimal_value << " = " << -eval - optimal_value << endl;
		}
	}
	else if (algorithm == "brkga") {
		cout << "BRKGA:" << endl;
		
		brkga<> br
		(
			br_params.POPULATION_SIZE,
			br_params.MUTANT_POPULATION_SIZE,
			br_params.ELITE_SET_SIZE,
			br_params.NUM_GENERATIONS,
			s->get_n_cities(),
			br_params.INHER_PROB
		);
		
		if (seed) {
			br.seed();
		}
		
		double eval;
		br.execute_algorithm(s, eval);
		
		//cout << "    BRKGA solution:" << endl;
		//s->print("    ");
		
		bool sane = s->sanity_check();
		cout << endl << "    Is final solution sane? " << (sane ? "Yes" : "No") << endl;
		cout << "    Final solution's cost: " << -eval << endl;
		if (use_optimal_value) {
			cout << "        Gap = BRKGA - ILP = " << -eval << " - " << optimal_value << " = " << -eval - optimal_value << endl;
		}
	}
	
	/* ******************************** */
	// --------- FREE MEMORY ---------- //
	/* ******************************** */
	
	fin.close();
	delete s;
}

