#
# Makefile
#

# Folders
SRC = src

# this is the directory I will compile from (i.e. the argument to -I)
IDIR := include

# all object files will be put here
OBJDIR := obj

# Compiler (g++)
CCC = g++

# Compiling flags
CCFLAGS +=  -Wno-deprecated-declarations -Wall -Wextra -pedantic -std=c++1z -Weffc++
GLFLAGS += -lGL -lX11 -lpthread -ldl
GLFWFLAGS += -lglfw -lGL

# Object modules
OBJECTS =  $(OBJDIR)/main.o $(OBJDIR)/glad.o $(OBJDIR)/stb_image.o 

# Main objetice - created with 'make' or 'make main'.
main: $(OBJECTS) makefile
	$(CCC) -I$(IDIR) $(CCFLAGS) -o main $(OBJECTS) $(GLFLAGS) $(GLFWFLAGS)

# Part objectives
$(OBJDIR)/main.o: $(SRC)/main.cc
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/main.cc -o $(OBJDIR)/main.o

$(OBJDIR)/glad.o: $(SRC)/glad.c
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/glad.c -o $(OBJDIR)/glad.o

$(OBJDIR)/stb_image.o: $(SRC)/stb_image.cc
	$(CCC) -I$(IDIR) $(CCFLAGS) -c $(SRC)/stb_image.cc -o $(OBJDIR)/stb_image.o

.PHONY: run dir clean zap
run: main
	./main

dir:
	@mkdir -p $(OBJDIR)

# 'make clean' removes object files and memory dumps.
clean:
	@ \rm -rf $(OBJDIR)/*.o *.gch core

# 'make zap' also removes the executable and backup files.
zap: clean
	@ \rm -rf main *~
