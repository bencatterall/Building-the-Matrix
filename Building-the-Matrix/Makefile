CXX = clang++
CXXFLAGS = -std=c++11 -Wall --pedantic -I ../Building-the-Matrix/Dependencies
# DEPS = GameObject.hpp UpdateManager.hpp Sender.hpp SafeQueue.hpp SafeMap.hpp Update.hpp Controls.hpp Message.hpp Serializer.hpp Sender.hpp Player.hpp LocationComponent.hpp ClientState.hpp
# DEPS += Controls.hpp Game.hpp GameObject.hpp ObjectManager.hpp Player.hpp Serializer.hpp SimplexNoise.hpp
DEPS =  
DEPS += Display.hpp
# DEPS += Chunk.hpp Cube.hpp ImageLoader.hpp LocationComponent.hpp RenderableComponent.hpp
DEPS += Client/Address.hpp Client/Socket.hpp
DEPS += Physics/PhysicsObject.hpp Physics/Simulator.hpp Physics/AABB.hpp Physics/PhysicsMaths.hpp
OBJ := $(DEPS:%.hpp=bin/%.o)
DEPS += Common.hpp
DEPS_SRC := $(DEPS:%=src/%)

all: bin/main

bin/main: bin/%: src/%.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

bin/%.o: src/%.cpp $(DEPS_SRC)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm bin/*
