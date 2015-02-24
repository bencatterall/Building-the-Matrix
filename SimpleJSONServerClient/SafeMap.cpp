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
	(this->lock).unlock();
	std::cout << "thread deleted " << id << "\n";
}

template<> void SafeMap<GameObjectGlobalID, GameObject>::put(GameObjectGlobalID id, std::shared_ptr<GameObject> object) {
	(this->lock).lock();
	auto iter = (this->map).insert(std::pair<GameObjectGlobalID, std::shared_ptr<GameObject>>(id, object));
	if (!iter.second) {
		//already existed
		//update the pointer
		this->map.at(id) = object;
		(this->lock).unlock();
		std::cout << "DUPLICATE" << std::endl;
	}
	else {
		(this->lock).unlock();
	}
	std::cout << "thread inserted " << id << "\n";
}

//TODO: RETURN POINTER IN THE MAP
template<> std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> SafeMap<GameObjectGlobalID, GameObject>::getSnapshot(bool flush) {
	(this->lock).lock();
	//return a copy of the map to send to the clients
	std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> m = (this->map);
	std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> newMap;
	std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>::iterator it;
	for (it = m.begin(); it != m.end(); it++) {
		std::shared_ptr<GameObject> copy;
		if ((it->second)->userControllable) {
			//is really a Player object so copy that
			std::shared_ptr<Player> playerOrig = std::dynamic_pointer_cast<Player>(it->second);
			copy = std::make_shared<Player>(*playerOrig);
		}
		else {
			//copy the Game Object
			copy = std::make_shared<GameObject>(*(it->second));
		}
		newMap.insert(std::pair<GameObjectGlobalID, std::shared_ptr<GameObject>>(it->first, copy));
	}
	if (flush) {
		//flush map
		(this->map) = std::map<GameObjectGlobalID, std::shared_ptr<GameObject>>();
	}
	(this->lock).unlock();
	return newMap;
}