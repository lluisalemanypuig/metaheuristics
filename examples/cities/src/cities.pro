TEMPLATE = app

CONFIG += c++1z console
CONFIG -= app_bundle
CONFIG -= qt
QT     -= core gui

QMAKE_CXXFLAGS_DEBUG += -DDEBUG -D_GLIBCXX_DEBUG
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3 -DNDEBUG -fstrict-aliasing

QMAKE_CXXFLAGS_DEBUG += -DLOCAL_SEARCH_DEBUG -DLOCAL_SEARCH_VERBOSE
QMAKE_CXXFLAGS_DEBUG += -DGRASP_DEBUG -DGRASP_VERBOSE
QMAKE_CXXFLAGS_DEBUG += -DGENETICS_DEBUG -DGENETICS_VERBOSE

QMAKE_CXXFLAGS +=										\
	-Wpedantic -Wshadow -Wall -Wextra -Wconversion		\
	-Wold-style-cast -Wrestrict -Wduplicated-cond		\
	-Wnon-virtual-dtor -Woverloaded-virtual -Wshadow

# metaheuristics library
MH_DIR = ../../../../metaheuristics
CONFIG(debug, debug|release) {
	LIBS += -L$${MH_DIR}/mh-debug/ -lmetaheuristicsdebug
	PRE_TARGETDEPS += $${MH_DIR}/mh-debug/libmetaheuristicsdebug.a
}
CONFIG(release, debug|release) {
	LIBS += -L$${MH_DIR}/mh-release/ -lmetaheuristics
	PRE_TARGETDEPS += $${MH_DIR}/mh-release/libmetaheuristics.a
}
INCLUDEPATH += $${MH_DIR}/
DEPENDPATH += $${MH_DIR}/

SOURCES += \
	centre_type.cpp \
	city.cpp \
	interface.cpp \
	impl/interface_constraints.cpp \
	impl/interface_inout.cpp \
	impl/interface_memory.cpp \
	solver.cpp \
	impl/solver_decode.cpp \
	impl/solver_greedy_construct.cpp \
	impl/solver_greedy_costs.cpp \
	impl/solver_neighbourhood.cpp \
	impl/solver_random_construct.cpp \
	main.cpp \
	point.cpp

HEADERS += \
	centre_type.hpp \
	city.hpp \
	interface.hpp \
	point.hpp \
	solver.hpp
