CXXFLAGS=-Wall -Wextra
LFLAGS=-lglfw
CXX=g++

SRC_FILES=main.cpp

run: build
	./bin/caustics

build:
	$(CXX) $(CXXFLAGS) $(SRC_FILES) $(LFLAGS) -o ./bin/caustics

clean:
	/bin/rm ./bin/caustics
