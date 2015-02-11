CXX = g++
CXXFLAGS = -std=c++11 -Wall --pedantic
DEPS = Address.hpp Socket.hpp
OBJ = Address.o Socket.o

all: ServerMain

ServerMain: %: %.cpp $(OBJ) 
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm *.o ServerMain
