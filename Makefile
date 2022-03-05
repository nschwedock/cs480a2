#Nathan Schwedock
#822910056
# CXX Make variable for compiler
CXX=g++
# Make variable for compiler options
#	-std=c++11  C/C++ variant to use, e.g. C++ 2011
#	-g          include information for symbolic debugger e.g. gdb 
CXXFLAGS=-std=c++11

# Rules format:
# target : dependency1 dependency2 ... dependencyN
#     Command to make target, uses default rules if not specified

# First target is the one executed if you just type make
# make target specifies a specific target
# $^ is an example of a special variable.  It substitutes all dependencies
dicttest : dicttest.o dicttree.o populatetree.o countwords.o
	$(CXX) $(CXXFLAGS) -g -o dicttest $^ -lpthread

dicttest.o : dicttest.cpp populatetree.h countwords.h exec_status.h

dicttree.o : dicttree.cpp dicttree.h

populatetree.o : populatetree.cpp populatetree.h exec_status.h

countwords.o : countwords.cpp countwords.h exec_status.h

clean :
	rm *.o