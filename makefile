CXXFLAGS=-Wall -Wextra -DGL_GLEXT_PROTOTYPES
LFLAGS=-lglfw
CXX=g++

SRC_FILES=main.cpp shader.cpp application.cpp

build:
	$(CXX) $(CXXFLAGS) $(SRC_FILES) $(LFLAGS) -o ./bin/caustics

run: build
	./bin/caustics

clean:
	/bin/rm ./bin/caustics
