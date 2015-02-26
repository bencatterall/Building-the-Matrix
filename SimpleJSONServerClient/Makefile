CXX = clang++
CXXFLAGS = -std=c++11 -Wall --pedantic -I ../Building-the-Matrix/Dependencies
DEPS = Address.hpp Socket.hpp GameObject.hpp UpdateManager.hpp Sender.hpp SafeQueue.hpp SafeMap.hpp Update.hpp Controls.hpp Message.hpp Serializer.hpp Sender.hpp Player.hpp LocationComponent.hpp ClientState.hpp
DEPS += Physics/PhysicsObject.hpp Physics/Simulator.hpp Physics/AABB.hpp Physics/PhysicsMaths.hpp
OBJ := $(DEPS:%.hpp=bin/%.o)
DEPS += CommonMinimal.hpp Common.hpp

all: bin/Server bin/test

bin/test bin/Server: bin/%: %.cpp $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm *.o ServerMain
