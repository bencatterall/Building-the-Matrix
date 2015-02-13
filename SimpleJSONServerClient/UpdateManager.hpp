#include "SafeMap.hpp"
#include "SafeQueue.hpp"
#include "Message.hpp"

class UpdateManager {
	private:
		SafeMap<GameObjectGlobalID, GameObject> updatedObjectsForClients;
		SafeMap<GameObjectGlobalID, GameObject> gameObjectsWorldState;
		SafeQueue<Update> pendingUpdates;
		bool cont;
		void updateObject(Update u);

	public:
		UpdateManager(std::vector<GameObject> initialGameObjects);
		~UpdateManager();
		//for physics to call
		void queueUpdate(Update update);
		//for received messages from clients
		void queueUpdates(Message updates);
		std::map<GameObjectGlobalID, GameObject> flushUpdates();
		std::map<GameObjectGlobalID, GameObject> getState();
		void run();
		void stop();
};