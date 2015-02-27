#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <memory>
#include <map>

class PhysicsObject;
#include "../World/Chunk.hpp"

class Simulator
{
public:
	static Simulator & getInstance();
	void tick(float);
	void setChunk(std::shared_ptr<Chunk>);

	int tickCount;
protected:

private:
	Simulator();
	~Simulator();
	void processCollisions(std::map<GameObjectGlobalID, std::shared_ptr<GameObject> >);

	std::shared_ptr<Chunk> chunk;
	static Simulator instance;
	float accumulator;
};

#endif