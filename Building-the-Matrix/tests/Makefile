CXX = g++
BIN_DIR = ../bin/
SRC_DIR = ../src/
CXXFLAGS = -std=c++11 -Wall --pedantic -I../Dependencies
# CXXFLAGS += -DTEST=1 -DTEST_UPDATES=1
# SRCS = Client/Address.hpp Socket.hpp GameObject.hpp UpdateManager.hpp Sender.hpp SafeQueue.hpp SafeMap.hpp Update.hpp Controls.hpp Message.hpp CommonMinimal.hpp
# SRCS = Game.hpp Cube.hpp GameObject.hpp LocationComponent.hpp
SRCS = GameObject.hpp Physics/PhysicsObject.hpp
OBJ = $(SRCS:%.hpp=$(BIN_DIR)%.o)
DEPS = $(SRCS:%=$(SRC_DIR)%)

test: SerializerTest

SerializerTest: %: %.cpp $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $(BIN_DIR)$@ $^

../bin/%.o: ../src/%.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o
