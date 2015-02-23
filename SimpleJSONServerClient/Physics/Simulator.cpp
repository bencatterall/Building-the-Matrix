#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "Simulator.hpp"
#include "..\GameObject.hpp"
#include "..\Player.hpp"
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
		//THIS WAS CRASHING WHEN GAME OBJECTS SIZE WAS 0
		std::map<GameObjectGlobalID, GameObject>::iterator it;
		//for (size_t i = 0; i < gameObjects.size(); i++)
		for (it = gameObjects.begin(); it != gameObjects.end();it++)
			{
				//GameObject & gameObj = gameObjects.at(i);
				GameObject gameObj = (it->second);
				//PhysicsObject physObj = *gameObj.physComp;
				PhysicsObject physObj = *gameObj.physComp;
				PhysicsMaths::stepObject(physObj, THRESHOLD);
				if (gameObj.userControllable){
					Player& player = (Player&)gameObj;
					bool *keys = player.getKeysPressed();
					if (keys[0]){
						PhysicsMaths::acceleratePlayer(player.ID);
					}
					if (keys[1]){
						PhysicsMaths::reversePlayer(player.ID);
					}
					if (keys[2] && !keys[3]){
						PhysicsMaths::turnRight(player.ID);
					}
					if (keys[3] && !keys[2]){
						PhysicsMaths::turnLeft(player.ID);
					}
				}
			}
		processCollisions();
	}
}

void Simulator::processCollisions(){
	UpdateManager& objMan = UpdateManager::getInstance();
	auto gameObjects = objMan.getState();

	// O(n^2) collision check
	//THIS WAS CRASHING WHEN GAME OBJECTS SIZE WAS 0
	if (gameObjects.size() > 0) {
		std::map<GameObjectGlobalID, GameObject>::iterator it;
		std::map<GameObjectGlobalID, GameObject>::iterator it2;
		//for (size_t i = 0; i < gameObjects.size(); i++)
		for (it = gameObjects.begin(); it != gameObjects.end(); it++)
		{
			//GameObject & gameObj = gameObjects.at(i);
			GameObject gameObj = (it->second);
			PhysicsObject currentObj = *gameObj.physComp;
			//for (size_t = i; j < gameObjects.size(); j++)
			for (it2 = it; it2 != gameObjects.end(); it2++)
			{
				GameObject gameObj2 = (it2->second);
				PhysicsObject checkObj = *gameObj2.physComp;
				if (PhysicsMaths::simpleCollision(currentObj, checkObj) && PhysicsMaths::complexCollision(gameObj.getID(), gameObj2.getID())){
					PhysicsMaths::handleCollision(gameObj, gameObj2);
				}
			}
		}
	}
}