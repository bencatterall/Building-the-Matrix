CXX = g++
CXXFLAGS = -std=c++11 -Wall --pedantic
CXXFLAGS += -I../../Dependencies
DEPS = Address.hpp Socket.hpp Client.hpp
OBJ = Address.o Socket.o clientMain.o Client.o MessageConstants.o

tests: TestClientNetworking
	./$^

all: clientMain

TestClientNetworking clientMain: %: %.cpp $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS)

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

clean:
	rm *.o
