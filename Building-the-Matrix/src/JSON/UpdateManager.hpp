#ifndef UPDATE_MANAGER_H
#define UPDATE_MANAGER_H
/*
UpdateManager will:
- 1) Receive and deserialise Game Inputs from clients. Presumably this is data held
in Game Objects which will be updated, so Physics can work out the new object
positions.

- 2) Receive updated location data from Physics, update the
releveant objects, serialise the changes and send them back to clients.

=> so need to have a GameInput objects for Game Inputs

=> In GameObject, we have:
	Global ObjectID (which the server will keep a map of)
	Local ObjectIDs for the benefit of updates sent back to client?
	LocationComponent, for position data
	GameInput object here?
	
	Need:

	- GameObject map
	- methods to add/remove GameObjects to/from map
	- serializing GameObject position updates (received from Physics)
	- deserializing GameInput commands (to be sent to Physics)

*/

#include "../Common.hpp"
#include "../GameObject.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

/*
Because it's the same principle a lot of the definitions here are the same/analagous to ObjectManager 
*/

class UpdateManager {

	// The ID of the next object to be loaded
	static int nextID;
	static std::mutex updManagerIndexMutex;

	~UpdateManager() {}
public:

	static UpdateManager& getInstance() {
		static UpdateManager instance;
		return instance;
	}

	//Set the global ID of a GameObject
	void registerObject(GameObjectID gameObjectID, GameObjectGlobalID globalID);
	
	//Serializes a game object to send its updated position to the client
	StringBuffer SerializeServer(GameObjectID gameObjectID);

	//Deserialize game inputs received from the client
	void DeserializeServer(const char* jsonStr);

	//Converse client side methods to send serialized inputs to server, and receive deserialized game objects from server 
	//StringBuffer SerializeClient(GameInput* gameInput)
	//void DeserializeClient(const char* jsonStr);

};

#endif