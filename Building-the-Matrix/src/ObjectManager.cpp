#include "ObjectManager.hpp"

#include <algorithm>

int ObjectManager::nextID = 1;

std::mutex ObjectManager::objManagerIndexMutex;

GameObjectID ObjectManager::getObjectLocalFromGlobalID(GameObjectGlobalID id) {
	//Find the object in the map
	auto search = gameObjectsGlobalToLocal.find(id);
	if (search != gameObjectsGlobalToLocal.end()) {
		return search->second;
	}
	else {
		//There is very likely a bug somewhere as an invalid ID has been used
		//assert(search != gameObjectsGlobalToLocal.end());
		//exit(EXIT_FAILURE);
		return 0; // NO ID
	}
}

bool ObjectManager::setObjectGlobal(GameObjectGlobalID globalID, GameObjectID localID) {
	auto iter = gameObjectsGlobalToLocal.insert(std::pair<GameObjectGlobalID, GameObjectID>(globalID, localID));
	if (!iter.second) {
		//already existed
		//update the pointer
		gameObjectsGlobalToLocal.at(globalID) = localID;
	}
	return true;
}

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
		//assert(search != gameObjects.end());
		//exit(EXIT_FAILURE);
	}
}

bool ObjectManager::exists(GameObjectID id)
{
	if (id == 0)
		return false;
	//Find the object in the map
	auto search = gameObjects.find(id);
	return (search != gameObjects.end());
}

bool ObjectManager::removeObject(GameObjectID id) {
	//Find it and then remove it
	if (exists(id)) {
		auto search = gameObjects.find(id);
		if (search != gameObjects.end()) {
			gameObjectIDs.erase(std::remove(gameObjectIDs.begin(), gameObjectIDs.end(), id), gameObjectIDs.end());
			gameObjects.erase(search);
			return true;
		}
		else {
			//There is very likely a bug somewhere as an invalid ID has been used
			//assert(search != gameObjects.end());
			//exit(EXIT_FAILURE);
		}
	}
};