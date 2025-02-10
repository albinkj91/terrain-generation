# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Wextra -Werror -Weffc++ -pedantic -Wno-deprecated-declarations 

# Directories
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Output executable
TARGET = $(BIN_DIR)/program

# Source files
MAIN_SRC = $(SRC_DIR)/main.cc
CLASS_SRC = $(filter-out $(MAIN_SRC), $(wildcard $(SRC_DIR)/*.cc))
CLASS_OBJ = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(CLASS_SRC))
MAIN_OBJ = $(patsubst $(SRC_DIR)/%.cc, $(OBJ_DIR)/%.o, $(MAIN_SRC))
OBJ_FILES = $(CLASS_OBJ) $(MAIN_OBJ)

# Dependency files
DEP_FILES = $(CLASS_OBJ:.o=.d)

# Default target
.PHONY: all
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files to object files (for classes)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc $(INCLUDE_DIR)/%.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Compile main.cc separately
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Include dependency files
-include $(DEP_FILES)

# Generate dependency files
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cc
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -MM -MP -MT "$(@:.d=.o) $@" $< > $@

# Clean up build files
.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Run the program
.PHONY: run
run: $(TARGET)
	./$(TARGET) && feh perlin.ppm

