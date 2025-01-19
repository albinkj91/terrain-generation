# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS +=  -Wno-deprecated-declarations -Wall -Wextra -pedantic -std=c++17 -Weffc++

# Part objectives
main:  main.cc
	$(CCC) $(CCFLAGS) main.cc -o main
