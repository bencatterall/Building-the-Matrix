#ifndef OBJECT_MANAGER_H
#define OBJECT_MANAGER_H

#include "Common.hpp"
#include "GameObject.hpp"
#include <map>
#include <memory>
#include <mutex> //std::mutex, std::lock_guard

///
///
///
class ObjectManager {
	///
	/// Map of GameObject IDs to GameObjects 
	///
	std::map<int, std::shared_ptr<GameObject>> gameObjects;

	///
	/// The ID of the next object to be loaded
	///
	static int nextID;
	static std::mutex objManagerIndexMutex;

public:

	static ObjectManager& getInstance() {
		//C++11, section 6.7, concurrent initialisation of statics is thread-safe
		//so we don't need any locking mechanisms.
		//http://stackoverflow.com/questions/449436/singleton-instance-declared-as-static-variable-of-getinstance-method/449823#449823
		static ObjectManager instance;
		return instance;
	}

	///
	/// Get the next available Globall Unique ID (GUID) for the new object
	/// Thread-safe
	/// An ID of 0 is invalid
	///
	static int getNextID() {
		int index = 0;
		try {
			std::lock_guard<std::mutex> lock(objManagerIndexMutex);
			index = nextID++;
		}
		catch (...) {
#ifdef DEBUG
			std::cerr << "ERROR: In generating object ID" << std::endl;
#endif
		}
		return index;
	}
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