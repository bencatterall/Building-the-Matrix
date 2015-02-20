#include "SafeMap.hpp"
#include "Player.hpp"
#include <iostream>

template<> SafeMap<GameObjectGlobalID,GameObject>::SafeMap() {
	(this->map) = std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>();
}

template<> SafeMap<GameObjectGlobalID, GameObject>::~SafeMap() {}

template<> std::shared_ptr<GameObject> SafeMap<GameObjectGlobalID, GameObject>::get(GameObjectGlobalID id) {
	(this->lock).lock();
	std::shared_ptr<GameObject> o = ((this->map).find(id))->second;
	(this->lock).unlock();
	return o;
}

template<> void SafeMap<GameObjectGlobalID, GameObject>::deleteEntry(GameObjectGlobalID id) {
	(this->lock).lock();
	(this->map).erase(id);
	std::cout << "thread deleted " << id << "\n";
	(this->lock).unlock();
}

template<> void SafeMap<GameObjectGlobalID, GameObject>::put(GameObjectGlobalID id, std::shared_ptr<GameObject> object) {
	(this->lock).lock();
	(this->map).insert(std::pair<GameObjectGlobalID, std::shared_ptr<GameObject>>(id, object));
	std::cout << "thread inserted " << id << "\n";
	(this->lock).unlock();
}

template<> std::map<GameObjectGlobalID, GameObject> SafeMap<GameObjectGlobalID, GameObject>::getSnapshot(bool flush) {
	(this->lock).lock();
	//return a copy of the map to send to the clients
	std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> m = (this->map);
	std::map<GameObjectGlobalID, GameObject> staticMap;
	std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {
		GameObject copy;
		if ((it->second)->userControllable) {
			//is really a Player object so copy that
			std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>((it->second));
			copy = Player(*player);
		}
		else {
			//copy the Game Object
			copy = GameObject(*(it->second));
		}
		staticMap.insert(std::pair<GameObjectGlobalID, GameObject>(it->first, copy));;
	}
	if (flush) {
		//flush map
		(this->map) = std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>();
	}
	(this->lock).unlock();
	return staticMap;
}