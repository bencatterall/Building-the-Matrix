#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "Simulator.hpp"
#include "..\GameObject.hpp"
#include "..\UpdateManager.hpp"

#define THRESHOLD 0.02f

Simulator::Simulator() :
	accumulator(0)
{
}


Simulator::~Simulator()
{
}

Simulator& Simulator::getInstance() {
	static Simulator instance;
	return instance;
}

void Simulator::tick(float timestep){
	UpdateManager& objMan = UpdateManager::getInstance();
	accumulator += timestep;
	while (accumulator > THRESHOLD){
		accumulator -= THRESHOLD;
		auto gameObjects = objMan.getState();

		// Step by THRESHOLD
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			GameObject & gameObj = gameObjects.at(i);
			PhysicsObject physObj = *gameObj.physComp;
			PhysicsMaths::stepObject(physObj, THRESHOLD);
		}

		processCollisions();
	}
}

void Simulator::processCollisions(){
	UpdateManager& objMan = UpdateManager::getInstance();
	auto gameObjects = objMan.getState();

	// O(n^2) collision check
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		GameObject & gameObj = gameObjects.at(i);
		PhysicsObject currentObj = *gameObj.physComp;
		for (size_t j = i; j < gameObjects.size(); j++)
		{
			GameObject gameObj2 = gameObjects.at(j);
			PhysicsObject checkObj = *gameObj2.physComp;
			/* TODO FIX ME
			if (PhysicsMaths::simpleCollision(currentObj, checkObj) && PhysicsMaths::complexCollision(gameObjects.at(i), gameObjects.at(j))){
				PhysicsMaths::handleCollision(gameObjects.at(i), gameObjects.at(j));
			}
			*/
		}
	}

}