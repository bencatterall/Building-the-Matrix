CXX = clang++
CXXFLAGS = -std=c++11 -Wall --pedantic -I../Building-the-Matrix/Dependencies
CXXFLAGS +=  -I ../Building-the-Matrix/LibOVR/Src -I ../Building-the-Matrix/LibOVR/Include
DEPS = Address.hpp Socket.hpp GameObject.hpp UpdateManager.hpp Sender.hpp SafeQueue.hpp SafeMap.hpp Update.hpp Controls.hpp Message.hpp Serializer.hpp Sender.hpp Player.hpp LocationComponent.hpp ClientState.hpp
DEPS += World/Chunk.hpp World/World.hpp
DEPS += Physics/PhysicsObject.hpp Physics/Simulator.hpp Physics/AABB.hpp Physics/PhysicsMaths.hpp
OBJ := $(DEPS:%.hpp=bin/%.o)
OBJ += bin/SimplexNoise.o
DEPS += CommonMinimal.hpp Common.hpp
DEPS += ../Building-the-Matrix/src/SimplexNoise.hpp

all: bin/Server bin/test

bin/test bin/Server: bin/%: %.cpp $(OBJ) 
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/SimplexNoise.o: ../Building-the-Matrix/src/SimplexNoise.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bin/%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm bin/*
	rm bin/Physics/*
