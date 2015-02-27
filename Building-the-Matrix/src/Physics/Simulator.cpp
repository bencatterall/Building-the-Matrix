#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "Simulator.hpp"
#include "../ObjectManager.hpp"
#include "../GameObject.hpp"

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
	ObjectManager& objMan = ObjectManager::getInstance();
	accumulator += timestep;
	while (accumulator > THRESHOLD){
		accumulator -= THRESHOLD;
		std::vector<GameObjectID> gameObjects = objMan.getObjects();

		// Step by THRESHOLD
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			std::shared_ptr<GameObject> gameObj = objMan.getObject(gameObjects.at(i));
			PhysicsObject physObj = *gameObj->getPhysicsComponent();
			PhysicsMaths::stepObject(physObj, THRESHOLD);
		}

		processCollisions();
	}
}

void Simulator::processCollisions(){
	ObjectManager& objMan = ObjectManager::getInstance();
	std::vector<GameObjectID> gameObjects = objMan.getObjects();

	// O(n^2) collision check
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		std::shared_ptr<GameObject> gameObj = objMan.getObject(gameObjects.at(i));
		PhysicsObject currentObj = *gameObj->getPhysicsComponent();
		for (size_t j = i; j < gameObjects.size(); j++)
		{
			std::shared_ptr<GameObject> gameObj2 = objMan.getObject(gameObjects.at(j));
			PhysicsObject checkObj = *gameObj2->getPhysicsComponent();
			if (PhysicsMaths::simpleCollision(currentObj, checkObj) && PhysicsMaths::complexCollision(gameObjects.at(i), gameObjects.at(j))){
				PhysicsMaths::handleCollision(gameObjects.at(i), gameObjects.at(j));
			}
		}
	}

}