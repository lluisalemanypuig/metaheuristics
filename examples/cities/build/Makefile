
# ---------------
# -- VARIABLES --

# DIRECTORIES
PROJ_DIR = ..

SRC_DIR = $(PROJ_DIR)/src
IMPL_DIR = $(SRC_DIR)/impl
RULES_DIR = $(PROJ_DIR)/build

DEB_BIN_DIR = $(PROJ_DIR)/cities-debug
REL_BIN_DIR = $(PROJ_DIR)/cities-release

METAHEURISTICS_DIR = ../../../

# FLAGS
GEN_FLAGS = -std=c++11 -Wunused -Wall
INCLUDE = -I$(SRC_DIR) -I$(METAHEURISTICS_DIR)
LIBS = -L../../../metaheuristics-release -lmetaheuristics

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
