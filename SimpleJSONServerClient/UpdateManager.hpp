#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "SafeMap.hpp"
#include "SafeQueue.hpp"
#include "Message.hpp"

class UpdateManager {
	private:
		SafeMap<GameObjectGlobalID, GameObject> updatedObjectsForClients;
		SafeMap<GameObjectGlobalID, GameObject> gameObjectsWorldState;
		SafeQueue<Update> pendingUpdates;
		bool cont;
		GameObjectGlobalID nextID;
		UpdateManager();
		~UpdateManager();
		static UpdateManager man;

	public:
		static UpdateManager& getInstance();
		void setInitialObjects(std::vector<std::shared_ptr<GameObject>> initialGameObjects);
		UpdateManager(const UpdateManager& updateManager){} //don't want compiler generating this function as mutexes are uncopyable
		//for physics to call
		void queueUpdate(std::shared_ptr<GameObject> object);
		void remove(GameObjectGlobalID id);
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> flushUpdates();
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> getState();
		std::shared_ptr<GameObject> getGameObject(GameObjectGlobalID id);
		GameObjectGlobalID getNextObjectID();
		void run();
		void stop();
};
#endif