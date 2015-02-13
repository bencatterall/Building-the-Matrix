#include "SafeMap.hpp"
#include <iostream>

SafeMap<GameObjectGlobalID,GameObject>::SafeMap() {
	(this->map) = std::map<GameObjectGlobalID, GameObject>();
}

SafeMap<GameObjectGlobalID, GameObject>::~SafeMap() {}

GameObject SafeMap<GameObjectGlobalID, GameObject>::get(GameObjectGlobalID id) {
	(this->lock).lock();
	GameObject o = ((this->map).find(id))->second;
	(this->lock).unlock();
	return o;
}

void SafeMap<GameObjectGlobalID, GameObject>::deleteEntry(GameObjectGlobalID id) {
	(this->lock).lock();
	(this->map).erase(id);
	std::cout << "thread deleted " << id << "\n";
	(this->lock).unlock();
}

void SafeMap<GameObjectGlobalID, GameObject>::put(GameObjectGlobalID id, GameObject object) {
	(this->lock).lock();
	(this->map).insert(std::pair<GameObjectGlobalID, GameObject>(id, object));
	std::cout << "thread inserted " << id << "\n";
	(this->lock).unlock();
}

std::map<GameObjectGlobalID, GameObject> SafeMap<GameObjectGlobalID, GameObject>::getSnapshot() {
	(this->lock).lock();
	std::map<GameObjectGlobalID, GameObject> m = (this->map);
	std::map<GameObjectGlobalID, GameObject>::iterator it;
	std::cout << "thread found snapshot:\n";
	for (it = m.begin(); it != m.end(); it++) {
		std::cout << "(" << ((*it).first) << ")\t";
	}
	std::cout << "\n";
	//flush map
	(this->map) = std::map<GameObjectGlobalID, GameObject>();
	(this->lock).unlock();
	return m;
}