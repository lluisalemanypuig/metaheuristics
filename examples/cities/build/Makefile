
# ---------------
# -- VARIABLES --

# DIRECTORIES
PROJ_DIR	= ..

SRC_DIR		= $(PROJ_DIR)/src
IMPL_DIR 	= $(SRC_DIR)/impl
RULES_DIR 	= $(PROJ_DIR)/build

DEB_BIN_DIR = $(PROJ_DIR)/bin-debug
REL_BIN_DIR = $(PROJ_DIR)/bin-release

METAHEURISTICS_DIR = ../../../

# FLAGS
GEN_FLAGS	= -std=c++11 -Wunused -Wall		\
			  -DLOCAL_SEARCH_VERBOSE		\
			  -DGRASP_VERBOSE				\
			  -DGENETICS_VERBOSE

INCLUDE		= -I$(SRC_DIR) -I$(METAHEURISTICS_DIR)
LIBS		= -L../../../metaheuristics-release -lmetaheuristics

# ------------------
# -- DEPENDENCIES --

include $(RULES_DIR)/Makefile.dependencies

# -----------
# -- RULES --

.PHONY: debugdirectory releasedirectory
debug: debugdirectory $(DEB_BIN_DIR)/cities
release: releasedirectory $(REL_BIN_DIR)/cities

include $(RULES_DIR)/Makefile.debug
include $(RULES_DIR)/Makefile.release
