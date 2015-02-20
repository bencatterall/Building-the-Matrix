#ifndef SERIALIZER_H
#define SERIALIZER_H
#include <string>
#include <map>
#include "GameObject.hpp"

class Serializer {
	
public:
	static float unpackFloat(const char* buffer, int &p);
	static int packFloat(char* buffer, float val);

	static int unpackInt(const char* buffer, int &p);
	static int packInt(char* buffer, int val);

	static std::string unpackString(const char* buffer, int &p);
	static int packString(char* buffer, std::string);

	static char unpackByte(const char* buffer, int &p);
	static int packByte(char* buffer, char byte);

    static std::map<GameObjectGlobalID, GameObject> unpackMap(
        char* buffer,
        int &p
    );
    static int packMap(
        char* buffer,
        std::map<GameObjectGlobalID, GameObject> objectMap
        // consider passing by reference to avoid a copy
    );
};
#endif