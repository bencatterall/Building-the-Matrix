#include "UpdateManager.hpp"

void UpdateManager::updateObject(Update u) {
	//TODO: make this atomic/how?
	(this->gameObjectsWorldState).put(u.getObjectID(), u.getEditedObject());
	(this->updatedObjectsForClients).put(u.getObjectID(), u.getEditedObject());
}

UpdateManager::UpdateManager(std::vector<GameObject> initialGameObjects) {
	for (GameObject go : initialGameObjects) {
		gameObjectsWorldState.put(go.getID(), go);
	}
	(this->cont) = true;
}

UpdateManager::~UpdateManager(){}

void UpdateManager::queueUpdate(Update update) {
	(this->pendingUpdates).pushToEnd(update);
}

void UpdateManager::queueUpdates(Message clientUpdate) {
	//TODO: Use Message function to parse the JSON then add each resulting update to the queue
}

std::map<GameObjectGlobalID, GameObject> UpdateManager::flushUpdates() {
	return (this->updatedObjectsForClients).getSnapshot();
}

std::map<GameObjectGlobalID, GameObject> UpdateManager::getState() {
	return (this->gameObjectsWorldState).getSnapshot();
}

void UpdateManager::run() {
	while (this->cont) {
		try {
			Update u = (this->pendingUpdates).popFromFront();
			updateObject(u);
		}
		catch (int& i) {
			//ignore, just don't need to carry out any operation if no updates are pending
		}
	}
}

void UpdateManager::stop() {
	(this->cont) = false;
}