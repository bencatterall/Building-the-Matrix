#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "Simulator.hpp"
#include "Graphics\ObjectManager.hpp"
#include "Graphics\GameObject.hpp"

#define THRESHOLD 0.01f

Simulator::Simulator()
{
	objMan = ObjectManager::getInstance();
	accumulator = 0;
}


Simulator::~Simulator()
{
}

Simulator & Simulator::getInstance() {
	static Simulator instance;
	return instance;
}

void Simulator::tick(float timestep){
	accumulator += timestep;
	while (accumulator > THRESHOLD){
		accumulator -= THRESHOLD;

		// Step by THRESHOLD
		for (int i = 0; i < objMan.objectCount();; i++)
		{
			GameObject gameObj = objMan::getObject(i);
			PhysicsObject physObj = gameObj.getPhysicsComponent();
			PhysicsMaths::stepObject(physObj, THRESHOLD);
		}

		processCollisions();
	}
}

void Simulator::processCollisions(){

}