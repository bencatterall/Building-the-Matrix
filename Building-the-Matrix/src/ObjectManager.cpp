#include "ObjectManager.hpp"

bool ObjectManager::addObject(std::shared_ptr<GameObject> gameObject){
	//Insert the object into the map
	auto ret = gameObjects.insert(
		std::pair< int, std::shared_ptr<GameObject>>( gameObject->getID(), gameObject));

	return ret.second;
}


std::shared_ptr<GameObject> ObjectManager::getObject(int id){
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


bool ObjectManager::removeObject(int id) {
	//Find it and then remove it
	auto search = gameObjects.find(id);
	if(search != gameObjects.end()) {
		gameObjects.erase(search);
		return true;
	}
	else {
		//There is very likely a bug somewhere as an invalid ID has been used
		assert(search != gameObjects.end());
		exit(EXIT_FAILURE);
	}
};