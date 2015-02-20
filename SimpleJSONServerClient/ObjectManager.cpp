#include "ObjectManager.hpp"

#include <algorithm>
#include <glm/mat4x4.hpp>

GameObjectID ObjectManager::nextID = 1;

std::mutex ObjectManager::objManagerIndexMutex;

bool ObjectManager::addObject(std::shared_ptr<GameObject> gameObject){
	//Insert the object into the map
	auto ret = gameObjects.insert(
		std::pair< int, std::shared_ptr<GameObject>>( gameObject->getID(), gameObject));

	gameObjectIDs.push_back(gameObject->getID());
	return ret.second;
}


std::shared_ptr<GameObject> ObjectManager::getObject(GameObjectID id){
	//Find the object in the map
	auto search = gameObjects.find(id);
	if(search != gameObjects.end()) {
		return search->second;
	} 
	else {
		//There is very likely a bug somewhere as an invalid ID has been used
		assert(search != gameObjects.end());
		exit(EXIT_FAILURE);
	}
}


bool ObjectManager::removeObject(GameObjectID id) {
	//Find it and then remove it
	auto search = gameObjects.find(id);
	if(search != gameObjects.end()) {
		gameObjectIDs.erase(std::remove(gameObjectIDs.begin(), gameObjectIDs.end(), id), gameObjectIDs.end());
		gameObjects.erase(search);
		return true;
	}
	else {
		//There is very likely a bug somewhere as an invalid ID has been used
		assert(search != gameObjects.end());
		exit(EXIT_FAILURE);
	}
};