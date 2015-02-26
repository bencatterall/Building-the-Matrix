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
accumulator(0), tickCount(0)
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
		tickCount++;
		accumulator -= THRESHOLD;
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> gameObjects = objMan.getState();

		// Step by THRESHOLD
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it;
		for (it = gameObjects.begin(); it != gameObjects.end();it++)
			{
			if (it->second->deleted){
				//std::cout << "Ignored deleted item"
				continue;
			}
				std::shared_ptr<GameObject> gameObj = (it->second);
				PhysicsMaths::stepObject(gameObj->physComp, THRESHOLD);
				if (gameObj->userControllable){
					std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(gameObj);
					bool up = player->getUp();
					bool down = player->getDown();
					bool left = player->getLeft();
					bool right = player->getRight();

					// Pressing anything
					if (up || down || left || right) {
						//std::cout << "keys presses:" << up << " " << down << " " << left << " " << right << "\n";
					}
					// Pressing nothing
					else{
						gameObj->physComp->setA(vec3());
						gameObj->physComp->setV(gameObj->physComp->getV()*(1.0f - gameObj->physComp->getLinDrag()));
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
					if (left && right && up && down){
						vec3 X = gameObj->physComp->getX();
						vec3 V = gameObj->physComp->getV();
						vec3 A = gameObj->physComp->getA();
						std::cout << "Object position:" << X.x << " " << X.y << " " << X.z << "\n";
						std::cout << "Object velocity:" << V.x << " " << V.y << " " << V.z << "\n";
						std::cout << "Object acceleration:" << A.x << " " << A.y << " " << A.z << "\n";
					}	
				}
			}
		processCollisions(gameObjects);
		for (it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			if (it->second->userControllable){
				//std::cout << "Physics: Sent item";
				if (!it->second->deleted){
					objMan.queueUpdate(it->second);
				}
			}
		}
	}
}

void Simulator::processCollisions(std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> gameObjects){
	// O(n^2) collision check
	if (gameObjects.size() > 0) {
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it;
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it2;
		for (it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			std::shared_ptr<GameObject> gameObj = (it->second);
			PhysicsObject currentObj = *(gameObj->physComp);
			it2 = it;
			for (it2++; it2 != gameObjects.end(); it2++)
			{
				std::shared_ptr<GameObject> gameObj2 = (it2->second);
				PhysicsObject checkObj = *(gameObj2->physComp);
				if (PhysicsMaths::simpleCollision(currentObj, checkObj)){ // && PhysicsMaths::complexCollision(gameObj->getID(), gameObj2->getID())){
					std::cout << "Collision between " << it->second->ID << " and " << it2->second->ID << "\n";
					PhysicsMaths::handleCollision(*gameObj, *gameObj2);
				}
			}
		}
	}
}