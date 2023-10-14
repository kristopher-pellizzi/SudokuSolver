ROOT_DIR := $(shell dirname '$(realpath $(firstword $(MAKEFILE_LIST)))')
SRC_DIR := $(ROOT_DIR)/src/
OBJ_DIR := $(ROOT_DIR)/obj/
MKDIR := @mkdir -p 
CXX := g++
COMP_FLAGS := -std=c++17 -Wall -Werror -c $(ADD_COMP_FLAGS)

.PHONY: all
all: solver

.PHONY: clean
clean:
	-rm cli_test
	-rm solver
	-rm -r obj
	$(MAKE) -C CppArgumentParser clean

.PHONY: test
test:
	@echo ROOT_DIR: $(ROOT_DIR)
	@echo SRC_DIR: $(SRC_DIR)
	@echo OBJ_DIR: $(OBJ_DIR)

$(OBJ_DIR): 
	$(MKDIR) $@
	@echo "Created folder $@"

$(OBJ_DIR)CliMain.o: $(SRC_DIR)CliMain.cpp $(ROOT_DIR)/CppArgumentParser/lib/libArgumentParser.so | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) -I$(ROOT_DIR)/CppArgumentParser/include $< -o $@

$(OBJ_DIR)CliTest.o: $(SRC_DIR)CliTest.cpp $(ROOT_DIR)/CppArgumentParser/lib/libArgumentParser.so | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) -I$(ROOT_DIR)/CppArgumentParser/include $< -o $@

$(OBJ_DIR)Grid.o: $(SRC_DIR)Grid.cpp $(SRC_DIR)Grid.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Cell.o: $(SRC_DIR)Cell.cpp $(SRC_DIR)Cell.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)CliView.o: $(SRC_DIR)CliView.cpp $(SRC_DIR)CliView.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@
	
$(OBJ_DIR)View.o: $(SRC_DIR)View.cpp $(SRC_DIR)View.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Coordinates.o: $(SRC_DIR)Coordinates.cpp $(SRC_DIR)Coordinates.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Solver.o: $(SRC_DIR)Solver.cpp $(SRC_DIR)Solver.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)TestSolver.o: $(SRC_DIR)TestSolver.cpp $(SRC_DIR)Solver.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Checkpoint.o: $(SRC_DIR)Checkpoint.cpp $(SRC_DIR)Checkpoint.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(ROOT_DIR)/CppArgumentParser/lib/libArgumentParser.so:
	$(MAKE) -C CppArgumentParser

solver: $(OBJ_DIR)CliMain.o $(OBJ_DIR)Checkpoint.o $(OBJ_DIR)Solver.o $(OBJ_DIR)Grid.o $(OBJ_DIR)View.o $(OBJ_DIR)CliView.o $(OBJ_DIR)Cell.o $(OBJ_DIR)Coordinates.o
	$(CXX) -L$(ROOT_DIR)/CppArgumentParser/lib -lArgumentParser $^ -o $@

# Uses an alternative solver class that allows to debug checkpointing system
test_solver: $(OBJ_DIR)CliMain.o $(OBJ_DIR)Checkpoint.o $(OBJ_DIR)TestSolver.o $(OBJ_DIR)Grid.o $(OBJ_DIR)View.o $(OBJ_DIR)CliView.o $(OBJ_DIR)Cell.o $(OBJ_DIR)Coordinates.o
	$(CXX) -L$(ROOT_DIR)/CppArgumentParser/lib -lArgumentParser $^ -o $@

# Uses an alternative main function that simply prints the grid filled with numbers
cli_test: $(OBJ_DIR)CliView.o $(OBJ_DIR)Grid.o $(OBJ_DIR)View.o $(OBJ_DIR)CliTest.o $(OBJ_DIR)Cell.o $(OBJ_DIR)Coordinates.o
	$(CXX) -L$(ROOT_DIR)/CppArgumentParser/lib -lArgumentParser $^ -o $@