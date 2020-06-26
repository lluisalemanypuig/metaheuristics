TEMPLATE = lib

CONFIG += c++11 static
CONFIG -= app_bundle
QT     -= core gui

CONFIG(release, debug|release) {
TARGET = metaheuristics
}
CONFIG(debug, debug|release) {
TARGET = metaheuristicsdebug
}

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG -fstrict-aliasing

QMAKE_CXXFLAGS +=										\
	-Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
	-Wold-style-cast -Wrestrict -Wduplicated-cond		\
	-Wnon-virtual-dtor -Woverloaded-virtual

INCLUDEPATH += ..

HEADERS += \
	algorithms.hpp \
	algorithms/genetic_algorithms/brkga.hpp \
	algorithms/genetic_algorithms/brkga.cpp \
	algorithms/genetic_algorithms/genetic_algorithm.hpp \
	algorithms/genetic_algorithms/genetic_algorithm.cpp \
	algorithms/genetic_algorithms/rkga.hpp \
	algorithms/genetic_algorithms/rkga.cpp \
	algorithms/grasp/grasp.hpp \
	algorithms/grasp/grasp.cpp \
	algorithms/local_search/local_search.hpp \
	algorithms/local_search/local_search.cpp \
	algorithms/metaheuristic.hpp \
	algorithms/metaheuristic.cpp \
	misc.hpp \
	misc/time.hpp \
	namespaces.hpp \
	random/random_generator.hpp \
	random/crandom_generator.cpp \
	random/drandom_generator.cpp \
	random/random_generator.cpp \
	structures.hpp \
	structures/chromosome.hpp \
	structures/individual.hpp \
	structures/infeasible_exception.hpp \
	structures/policies.hpp \
	structures/problem.hpp \
	structures/problem.cpp

SOURCES += \
	misc/time.cpp \
	structures/chromosome.cpp \
	structures/individual.cpp \
	structures/infeasible_exception.cpp
