ROOT_DIR := $(shell dirname '$(realpath $(firstword $(MAKEFILE_LIST)))')
SRC_DIR := $(ROOT_DIR)/src/
OBJ_DIR := $(ROOT_DIR)/obj/
MKDIR := @mkdir -p 
CXX := g++
COMP_FLAGS := -c

.PHONY: all
all: solver

.PHONY: clean
clean:
	-rm cli_test
	-rm solver
	-rm -r obj

.PHONY: test
test:
	@echo ROOT_DIR: $(ROOT_DIR)
	@echo SRC_DIR: $(SRC_DIR)
	@echo OBJ_DIR: $(OBJ_DIR)

$(OBJ_DIR): 
	$(MKDIR) $@
	@echo "Created folder $@"

$(OBJ_DIR)Solver.o: $(SRC_DIR)Solver.cpp | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)CliTest.o: $(SRC_DIR)CliTest.cpp | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Grid.o: $(SRC_DIR)Grid.cpp $(SRC_DIR)Grid.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Cell.o: $(SRC_DIR)Cell.cpp $(SRC_DIR)Cell.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)CliView.o: $(SRC_DIR)CliView.cpp $(SRC_DIR)CliView.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

solver: $(OBJ_DIR)Solver.o $(OBJ_DIR)Grid.o $(OBJ_DIR)CliView.o $(OBJ_DIR)Cell.o
	$(CXX) $^ -o $@

# Uses an alternative main function that simply prints the grid filled with numbers
cli_test: $(OBJ_DIR)CliView.o $(OBJ_DIR)Grid.o $(OBJ_DIR)CliTest.o $(OBJ_DIR)Cell.o
	$(CXX) $^ -o $@