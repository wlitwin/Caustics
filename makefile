CXXFLAGS=-Wall -Wextra -DGL_GLEXT_PROTOTYPES
LFLAGS=-lglfw
CXX=g++

BIN_DIR=./bin
OBJ_DIR=obj

OBJ_FILES=$(shell find src/ -name '*.cpp' | sed -e 's/^\(.*\).cpp$$/obj\/\1.o/g')

#==============================================================================
# RULES
#==============================================================================

$(OBJ_DIR)/%.o : %.cpp
	@echo Compiling $^
	$(shell mkdir -p $(dir $@))
	@$(CXX) $(CXXFLAGS) -c $^ -o $@

build: $(OBJ_FILES)
	@$(CXX) $(OBJ_FILES) $(LFLAGS) -o $(BIN_DIR)/caustics
	@echo Finished

run: build
	$(BIN_DIR)/caustics

clean:
	/bin/rm -f $(BIN_DIR)/caustics
	/bin/rm -rf `find $(OBJ_DIR)/ -name '*.o'`
