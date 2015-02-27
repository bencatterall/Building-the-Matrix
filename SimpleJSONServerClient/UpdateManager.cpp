#include "UpdateManager.hpp"
#include "Player.hpp"
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
	//std::cout << "pushed new edit\n";
	(this->pendingUpdates).pushToEnd(Update(object->getID(), object));
}

void UpdateManager::remove(GameObjectGlobalID id) {
	std::shared_ptr<GameObject> o = (this->gameObjectsWorldState).get(id);
	o->deleted = true;
	(this->updatedObjectsForClients).put(id, o);
	if ((this->gameObjectsWorldState).count(id) != 0) {
		(this->gameObjectsWorldState).deleteEntry(id);
	}
}

std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> UpdateManager::flushUpdates() {
	return (this->updatedObjectsForClients).getSnapshot(true);
}

std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> UpdateManager::getState() {
	return (this->gameObjectsWorldState).getSnapshot(false);
}

std::shared_ptr<GameObject> UpdateManager::getGameObject(GameObjectGlobalID id) {
	return (this->gameObjectsWorldState).get(id);
}

void UpdateManager::run() {
	while (this->cont) {
		if (!(this->pendingUpdates).isEmpty()) {
			Update u = (this->pendingUpdates).popFromFront();
			if ((this->gameObjectsWorldState).count(u.getObjectID()) == 1) {
				if (u.getEditedObject()->userControllable && !u.getEditedObject()->deleted) {
					std::shared_ptr<Player> old = std::dynamic_pointer_cast<Player>((this->gameObjectsWorldState).get(u.getObjectID()));
					std::shared_ptr<Player> newObj = std::dynamic_pointer_cast<Player>(u.getEditedObject());
					newObj->setPRY(old->getPitch(), old->getRoll(), old->getYaw());
					newObj->setControl(old->getControl());
					newObj->deleted = (old->deleted);
				}
			}
			if (!u.getEditedObject()->deleted) {
				(this->gameObjectsWorldState).put(u.getObjectID(), u.getEditedObject());
				(this->updatedObjectsForClients).put(u.getObjectID(), u.getEditedObject());
			}
			else {}
		}
	}
}

void UpdateManager::stop() {
	(this->cont) = false;
}