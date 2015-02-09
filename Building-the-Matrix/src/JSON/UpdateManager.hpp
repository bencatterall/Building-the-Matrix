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

#include "../GameObject.hpp"
#include <map>
#include <memory>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace rapidjson;

class UpdateManager {

	std::map < int, GameObject> gameObjects;

public:

	void addObject(GameObject gameObject);

	//GameObject findObject(int id);

	void removeObject(int id);

	StringBuffer Serialize(GameObject gameObject);

	void Deserialize(const char* jsonStr);

};

#endif