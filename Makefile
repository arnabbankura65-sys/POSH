CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wpedantic -Iinclude
LDFLAGS :=

SRC_DIR := src
OBJ_DIR := build
TEST_DIR := tests

SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_BIN := $(OBJ_DIR)/test_runner

all: posh

posh: $(OBJ_DIR)/main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

test: $(TEST_BIN)
	./$(TEST_BIN)

$(TEST_BIN): $(OBJECTS) $(TEST_SOURCES) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) $(TEST_SOURCES) -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ_DIR) posh

.PHONY: all posh test clean