ROOT_DIR := $(shell dirname '$(realpath $(firstword $(MAKEFILE_LIST)))')
SRC_DIR := $(ROOT_DIR)/src/
OBJ_DIR := $(ROOT_DIR)/obj/
INCLUDE_DIR := $(ROOT_DIR)/include/
MKDIR := @mkdir -p 
CXX := g++
COMP_FLAGS := -std=c++17 -Wall -Werror -Iinclude -c $(ADD_COMP_FLAGS)

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

$(OBJ_DIR)Grid.o: $(SRC_DIR)Grid.cpp $(INCLUDE_DIR)Grid.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)PathGridInitializer.o: $(SRC_DIR)PathGridInitializer.cpp $(INCLUDE_DIR)PathGridInitializer.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)RandomGridInitializer.o: $(SRC_DIR)RandomGridInitializer.cpp $(INCLUDE_DIR)RandomGridInitializer.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Cell.o: $(SRC_DIR)Cell.cpp $(INCLUDE_DIR)Cell.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)CliView.o: $(SRC_DIR)CliView.cpp $(INCLUDE_DIR)CliView.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@
	
$(OBJ_DIR)View.o: $(SRC_DIR)View.cpp $(INCLUDE_DIR)View.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Coordinates.o: $(SRC_DIR)Coordinates.cpp $(INCLUDE_DIR)Coordinates.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)LockedCandidateIndex.o: $(SRC_DIR)LockedCandidateIndex.cpp $(INCLUDE_DIR)LockedCandidateIndex.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Solver.o: $(SRC_DIR)Solver.cpp $(INCLUDE_DIR)Solver.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)TestSolver.o: $(SRC_DIR)TestSolver.cpp $(INCLUDE_DIR)Solver.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(OBJ_DIR)Checkpoint.o: $(SRC_DIR)Checkpoint.cpp $(INCLUDE_DIR)Checkpoint.h | $(OBJ_DIR)
	$(CXX) $(COMP_FLAGS) $< -o $@

$(ROOT_DIR)/CppArgumentParser/lib/libArgumentParser.so:
	$(MAKE) -C CppArgumentParser

solver: $(OBJ_DIR)CliMain.o $(OBJ_DIR)Checkpoint.o $(OBJ_DIR)LockedCandidateIndex.o $(OBJ_DIR)Solver.o $(OBJ_DIR)Grid.o $(OBJ_DIR)PathGridInitializer.o $(OBJ_DIR)RandomGridInitializer.o $(OBJ_DIR)View.o $(OBJ_DIR)CliView.o $(OBJ_DIR)Cell.o $(OBJ_DIR)Coordinates.o
	$(CXX) -L$(ROOT_DIR)/CppArgumentParser/lib -lArgumentParser $^ -o $@

# Uses an alternative solver class that allows to debug checkpointing system
test_solver: $(OBJ_DIR)CliMain.o $(OBJ_DIR)Checkpoint.o $(OBJ_DIR)LockedCandidateIndex.o $(OBJ_DIR)TestSolver.o $(OBJ_DIR)Grid.o $(OBJ_DIR)PathGridInitialized $(OBJ_DIR)RandomGridInitializer $(OBJ_DIR)View.o $(OBJ_DIR)CliView.o $(OBJ_DIR)Cell.o $(OBJ_DIR)Coordinates.o
	$(CXX) -L$(ROOT_DIR)/CppArgumentParser/lib -lArgumentParser $^ -o $@

# Uses an alternative main function that simply prints the grid filled with numbers
cli_test: $(OBJ_DIR)CliView.o $(OBJ_DIR)Grid.o $(OBJ_DIR)PathGridInitialized $(OBJ_DIR)RandomGridInitializer $(OBJ_DIR)View.o $(OBJ_DIR)CliTest.o $(OBJ_DIR)Cell.o $(OBJ_DIR)Coordinates.o
	$(CXX) -L$(ROOT_DIR)/CppArgumentParser/lib -lArgumentParser $^ -o $@