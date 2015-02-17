#include "UpdateManager.hpp"

void UpdateManager::updateObject(Update u) {
	//TODO: make this atomic/how?
	(this->gameObjectsWorldState).put(u.getObjectID(), u.getEditedObject());
	(this->updatedObjectsForClients).put(u.getObjectID(), u.getEditedObject());
}

UpdateManager::UpdateManager() {
	(this->cont) = true;
}

void UpdateManager::setInitialObjects(std::vector<GameObject> initialGameObjects) {
	for (GameObject go : initialGameObjects) {
		gameObjectsWorldState.put(go.getID(), go);
	}
}

UpdateManager::~UpdateManager(){}

void UpdateManager::queueUpdate(GameObject object) {
	(this->pendingUpdates).pushToEnd(Update(object.getID(), object));
}

void UpdateManager::remove(GameObjectGlobalID id) {
	GameObject o = (this->gameObjectsWorldState).get(id);
	(this->pendingUpdates).pushToEnd(Update(o.getID(), o, 1));
}

std::map<GameObjectGlobalID, GameObject> UpdateManager::flushUpdates() {
	return (this->updatedObjectsForClients).getSnapshot();
}

std::map<GameObjectGlobalID, GameObject> UpdateManager::getState() {
	return (this->gameObjectsWorldState).getSnapshot();
}

GameObject UpdateManager::getGameObject(GameObjectGlobalID id) {
	return (this->gameObjectsWorldState).get(id);
}

void UpdateManager::run() {
	while (this->cont) {
		try {
			Update u = (this->pendingUpdates).popFromFront();
			if (u.forDeletion()) {
				(this->gameObjectsWorldState).deleteEntry(u.getObjectID());
				//client will need to check for deletion too
				(this->updatedObjectsForClients).put(u.getObjectID(), u.getEditedObject());
			}
			else {
				updateObject(u);
			}
		}
		catch (int& i) {
			//ignore, just don't need to carry out any operation if no updates are pending
		}
	}
}

void UpdateManager::stop() {
	(this->cont) = false;
}