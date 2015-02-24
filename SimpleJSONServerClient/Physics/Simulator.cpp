#include <iostream>

#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "Simulator.hpp"
#include "../GameObject.hpp"
#include "../Player.hpp"
#include "../UpdateManager.hpp"

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
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> gameObjects = objMan.getState();

		// Step by THRESHOLD
		//THIS WAS CRASHING WHEN GAME OBJECTS SIZE WAS 0
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it;
		//for (size_t i = 0; i < gameObjects.size(); i++)
		for (it = gameObjects.begin(); it != gameObjects.end();it++)
			{
				//GameObject & gameObj = gameObjects.at(i);
				std::shared_ptr<GameObject> gameObj = (it->second);
				//PhysicsObject physObj = *gameObj.physComp;
				PhysicsObject physObj = *(gameObj->physComp);
				PhysicsMaths::stepObject(physObj, THRESHOLD);
				if (gameObj->userControllable){
					std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(gameObj);
					bool up = player->getUp();
					bool down = player->getDown();
					bool left = player->getLeft();
					bool right = player->getRight();
					if (up | down| left | right) {
						std::cout << "keys presses:" << up << " " << down << " " << left << " " << right << "\n";
					}
					if (up && !down){
						PhysicsMaths::acceleratePlayer(gameObj->physComp);
					}
					if (down && !up){
						PhysicsMaths::reversePlayer(gameObj->physComp);
					}
					if (right && !left){
						PhysicsMaths::turnRight(gameObj->physComp);
					}
					if (left && !right){
						PhysicsMaths::turnLeft(gameObj->physComp);
					}
					vec3 X = physObj.getX();
					//std::cout << "Object position:" << X.x << " " << X.y << " " << X.z;
				}
			}
		processCollisions(gameObjects);
		for (it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			if (it->second->userControllable){
				//std::cout << "Physics: Sent item";
				objMan.queueUpdate(it->second);
			}
		}
	}
}

void Simulator::processCollisions(std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> gameObjects){
	UpdateManager & objMan = UpdateManager::getInstance();
	// O(n^2) collision check
	//THIS WAS CRASHING WHEN GAME OBJECTS SIZE WAS 0
	if (gameObjects.size() > 0) {
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it;
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it2;
		//for (size_t i = 0; i < gameObjects.size(); i++)
		for (it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			//GameObject & gameObj = gameObjects.at(i);
			std::shared_ptr<GameObject> gameObj = (it->second);
			PhysicsObject currentObj = *(gameObj->physComp);
			//for (size_t = i; j < gameObjects.size(); j++)
			for ((it2 = it)++; it2 != gameObjects.end(); it2++)
			{
				std::shared_ptr<GameObject> gameObj2 = (it2->second);
				PhysicsObject checkObj = *(gameObj2->physComp);
				if (PhysicsMaths::simpleCollision(currentObj, checkObj)){ // && PhysicsMaths::complexCollision(gameObj->getID(), gameObj2->getID())){
					PhysicsMaths::handleCollision(*gameObj, *gameObj2);
				}
			}
		}
	}
}