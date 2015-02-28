#include <iostream>

#include "../../Building-the-Matrix/Dependencies/GLFW/glfw3.h"
#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "Simulator.hpp"
#include "../GameObject.hpp"
#include "../Player.hpp"
#include "../UpdateManager.hpp"
#include "../World/Chunk.hpp"


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

void Simulator::setChunk(std::shared_ptr<Chunk> newChunk){
	chunk = newChunk;
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
					bool up = player->getKey(GLFW_KEY_W);
					bool down = player->getKey(GLFW_KEY_S);
					bool left = player->getKey(GLFW_KEY_A);
					bool right = player->getKey(GLFW_KEY_D);
					bool ascend = player->getKey(GLFW_KEY_E);
					bool descend = player->getKey(GLFW_KEY_Q);
					
					bool boostKey = player->getKey(GLFW_KEY_C);
					bool emergencyStop = player->getKey(GLFW_KEY_X);
					bool resetX = player->getKey(GLFW_KEY_Z);
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
					if (boostKey){
						gameObj->physComp->setV(gameObj->physComp->getV() * 1.01f);
					}
					if (ascend && ! descend){
						gameObj->physComp->setX(gameObj->physComp->getX() + vec3(0.0f, 0.1f, 0.0f));
					}
					if (descend && !ascend){
						gameObj->physComp->setX(gameObj->physComp->getX() + vec3(0.0f, -0.1f, 0.0f));
					}
					if (emergencyStop){
						gameObj->physComp->setV(vec3());
					}
					if (resetX){
						gameObj->physComp->setX(vec3(0.0f, 50.0f, 0.0f));
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
			std::shared_ptr<PhysicsObject> currentObj = gameObj->physComp;
			it2 = it;
			for (it2++; it2 != gameObjects.end(); it2++)
			{
				std::shared_ptr<GameObject> gameObj2 = (it2->second);
				PhysicsObject checkObj = *(gameObj2->physComp);
				if (PhysicsMaths::simpleCollision(*currentObj, checkObj)){ // && PhysicsMaths::complexCollision(gameObj->getID(), gameObj2->getID())){
					std::cout << "Collision between " << it->second->ID << " and " << it2->second->ID << "\n";
					PhysicsMaths::handleCollision(*gameObj, *gameObj2);
				}
			}
			bool terrainCollision = false;
			auto worldAABB = currentObj->getWorldAABB()->getFullBox();
			for (size_t i = 0; i < 8; i++){
				vec3 currentVertex = worldAABB->at(i);
				if (chunk->cubeAt(currentVertex)){
					terrainCollision = true;
					break;
				}
			}
			terrainCollision = chunk->cubeAt(currentObj->getX());
			if (terrainCollision){
				vertexVector vectorAABB = std::vector<glm::vec3>(2);
				vectorAABB.at(0) = vec3(-chunk->getCubeSize() / 2.0f);
				vectorAABB.at(1) = vec3(chunk->getCubeSize() / 2.0f);
				vec3 pos = chunk->getCubeCenter(currentObj->getX());
				GameObject tmpObj = GameObject();
				tmpObj.locComp = std::make_shared<LocationComponent>(pos);
				auto tmpTerrain = std::make_shared<PhysicsObject>(tmpObj.locComp, vectorAABB);
				tmpTerrain->setMass(0.0f);
				tmpObj.physComp = tmpTerrain;
				std::cout << "Collision between " << it->second->ID << " and terrain\n";
				PhysicsMaths::handleCollision(*gameObj, tmpObj);
			}
		}
	}
}