CXX = clang++
CXXFLAGS = -std=c++11 -Wall --pedantic -I ../Building-the-Matrix/Dependencies
CXXFLAGS +=  -I ../Building-the-Matrix/LibOVR/Src -I ../Building-the-Matrix/LibOVR/Include
# LIBS_SRC = 
# LIBS = $(LIBS_SRC:%=../Building-the-Matrix/Dependencies/%)
# CXXFLAGS += -L $(LIBS)
DEPS =  
# DEPS += Display.hpp
DEPS += Controls.hpp Game.hpp GameObject.hpp ObjectManager.hpp Player.hpp
DEPS += Client/Address.hpp Client/Socket.hpp Client/Client.hpp Client/MessageConstants.hpp
DEPS += Serializer.hpp
# Graphics
DEPS += Chunk.hpp Cube.hpp ImageLoader.hpp LocationComponent.hpp RenderableComponent.hpp
DEPS += SimplexNoise.hpp TextRenderer.hpp shader.hpp TextureAtlas.hpp Texture.hpp
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
