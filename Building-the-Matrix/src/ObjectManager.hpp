#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "Common.hpp"
#include "GameObject.hpp"
#include <map>
#include <memory>
class ObjectManager {
	///
	/// Map of GameObject IDs to GameObjects 
	///
	std::map<int, std::shared_ptr<GameObject>> gameObjects;
public:
	///
	/// Add an object to the object Manager
	///
	bool addObject(std::shared_ptr<GameObject> gameObject);

	///
	/// Get an object by its ID
	///
	std::shared_ptr<GameObject> getObject(int id);

	///
	/// Remove an object from the object manager
	///
	bool removeObject(int id);
};

#endif