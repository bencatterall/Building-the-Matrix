#include "UpdateManager.hpp"
#include <iostream>

UpdateManager::UpdateManager() {
	(this->cont) = true;
	(this->nextID) = 1;
}

UpdateManager& UpdateManager::getInstance(){
	static UpdateManager man;
	return man;
}

void UpdateManager::setInitialObjects(std::vector<std::shared_ptr<GameObject>> initialGameObjects) {
	for (std::shared_ptr<GameObject> go : initialGameObjects) {
		gameObjectsWorldState.put(go->getID(), go);
	}
}

GameObjectGlobalID UpdateManager::getNextObjectID() {
	GameObjectGlobalID id = (this->nextID);
	(this->nextID)++;
	return id;
}

UpdateManager::~UpdateManager(){}

void UpdateManager::queueUpdate(std::shared_ptr<GameObject> object) {
	std::cout << "pushed new edit\n";
	(this->pendingUpdates).pushToEnd(Update(object->getID(), object));
}

void UpdateManager::remove(GameObjectGlobalID id) {
	std::shared_ptr<GameObject> o = (this->gameObjectsWorldState).get(id);
	o->deleted = true;
	(this->pendingUpdates).pushToEnd(Update(o->getID(), o));
}

std::map<GameObjectGlobalID, GameObject> UpdateManager::flushUpdates() {
	return (this->updatedObjectsForClients).getSnapshot(true);
}

std::map<GameObjectGlobalID, GameObject> UpdateManager::getState() {
	return (this->gameObjectsWorldState).getSnapshot(false);
}

std::shared_ptr<GameObject> UpdateManager::getGameObject(GameObjectGlobalID id) {
	return (this->gameObjectsWorldState).get(id);
}

void UpdateManager::run() {
	while (this->cont) {
		if (!(this->pendingUpdates).isEmpty()) {
			Update u = (this->pendingUpdates).popFromFront();
			if ((u.getEditedObject())->deleted) {
				(this->gameObjectsWorldState).deleteEntry(u.getObjectID());
			}
			(this->gameObjectsWorldState).put(u.getObjectID(),u.getEditedObject());
			//NOTE:client will need to check for deletion too
			(this->updatedObjectsForClients).put(u.getObjectID(), u.getEditedObject());
		}
	}
}

void UpdateManager::stop() {
	(this->cont) = false;
}