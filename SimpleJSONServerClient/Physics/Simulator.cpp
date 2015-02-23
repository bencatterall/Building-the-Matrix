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
		auto gameObjects = objMan.getState();

		// Step by THRESHOLD
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			GameObject & gameObj = gameObjects.at(i);
			PhysicsObject physObj = *gameObj.physComp;
			PhysicsMaths::stepObject(physObj, THRESHOLD);
			if (gameObj.userControllable){
				Player& player = (Player&) gameObj;
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
	for (size_t i = 0; i < gameObjects.size(); i++)
	{
		GameObject & gameObj = gameObjects.at(i);
		PhysicsObject currentObj = *gameObj.physComp;
		for (size_t j = i; j < gameObjects.size(); j++)
		{
			GameObject gameObj2 = gameObjects.at(j);
			PhysicsObject checkObj = *gameObj2.physComp;
			if (PhysicsMaths::simpleCollision(currentObj, checkObj) && PhysicsMaths::complexCollision(gameObj.getID(), gameObj2.getID())){
				PhysicsMaths::handleCollision(gameObj, gameObj2);
			}
		}
	}

}