
# ---------------
# -- VARIABLES --

# DIRECTORIES
PROJ_DIR = ..

SRC_DIR			= $(PROJ_DIR)/metaheuristics
STRUCT_DIR		= $(SRC_DIR)/structures
RND_DIR			= $(SRC_DIR)/random
MISC_DIR		= $(SRC_DIR)/misc
ALGS_DIR		= $(SRC_DIR)/algorithms
GEN_ALGS_DIR	= $(ALGS_DIR)/genetic_algorithms
GRASP_ALGS_DIR	= $(ALGS_DIR)/grasp
LS_ALGS_DIR		= $(ALGS_DIR)/local_search

RULES_DIR		= $(PROJ_DIR)/build

DEB_BIN_DIR		= $(PROJ_DIR)/metaheuristics-debug
REL_BIN_DIR		= $(PROJ_DIR)/metaheuristics-release

# FLAGS
GEN_FLAGS = -std=c++11 -Wunused -Wall
INCLUDE = $(PROJ_DIR)
LIBS = 

# ------------------
# -- DEPENDENCIES --

include $(RULES_DIR)/Makefile.dependencies

# -----------
# -- RULES --

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/libmetaheuristics.a
release: releasedirectory $(REL_BIN_DIR)/libmetaheuristics.a

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
