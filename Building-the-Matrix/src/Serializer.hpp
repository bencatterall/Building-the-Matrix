#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <string>
#include <map>
#include "GameObject.hpp"
#include "Common.hpp"

class Serializer {

public:
	static float unpackFloat(unsigned const char* buffer, int &p);
	static int packFloat(unsigned char* buffer, float val);

	static double unpackDouble(unsigned const char* buffer, int &p);
	static int packDouble(unsigned char* buffer, double val);

	static int unpackInt(unsigned const char* buffer, int &p);
	static int packInt(unsigned char* buffer, int val);

	static std::string unpackString(unsigned const char* buffer, int &p);
	static int packString(unsigned char* buffer, std::string);

	static char unpackByte(unsigned const char* buffer, int &p);
	static int packByte(unsigned char* buffer, char byte);

	static bool unpackBool(unsigned const char* buffer, int &p);
	static int packBool(unsigned char* buffer, bool boolean);

};
#endif