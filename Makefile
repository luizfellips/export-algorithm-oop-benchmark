# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
MAIN_DIR = .

# Files
MAIN_CPP = $(MAIN_DIR)/main.cpp
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(OBJ_DIR)/main.o $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CXX = g++
CXXFLAGS = -std=c++17 -I$(INC_DIR) -Wall -Wextra
LDFLAGS = -lmysqlcppconn

# Output file
OUTPUT = main

# Default target
all: $(OUTPUT)

$(OUTPUT): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(OUTPUT) $(LDFLAGS)

$(OBJ_DIR)/main.o: $(MAIN_CPP)
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $(MAIN_CPP) -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -rf $(OBJ_DIR) $(OUTPUT)

.PHONY: all clean
