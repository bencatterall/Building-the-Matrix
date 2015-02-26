#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <memory>
#include <map>

class PhysicsObject;

class Simulator
{
public:
	static Simulator & getInstance();
	void tick(float);
	int tickCount;
protected:

private:
	Simulator();
	~Simulator();
	void processCollisions(std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>);

	static Simulator instance;
	float accumulator;
};

#endif