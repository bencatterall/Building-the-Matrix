#include "Serializer.hpp"
#include "Common.hpp"
#include <cstdint>
///Unpack from network byte order
// p is the size of the float
float Serializer::unpackFloat(const char* buffer, int &p){
	uint32_t temp = 0;

	//4 bytes
	p += 4;

	temp = ((buffer[0] << 24) |
		(buffer[1] << 16) |
		(buffer[2] << 8) |
		buffer[3]);

	return *((float*)&temp);
}

int Serializer::packFloat(char* buffer, float val) {
#ifndef INTEL_X86
#error Not on Intel, serializing incorrect. Expecting little-endian.
#endif
	unsigned char* temp = (unsigned char *)&val;

	buffer[0] = temp[3];
	buffer[1] = temp[2];
	buffer[2] = temp[1];
	buffer[3] = temp[0];

	//4 bytes
	return 4;
}

int Serializer::unpackInt(const char* buffer, int &p){
	uint32_t temp = 0;

	//4 bytes
	p += 4;

	temp = ((buffer[0] << 24) |
		(buffer[1] << 16) |
		(buffer[0] << 8) |
		buffer[3]);

	return temp;
}

int Serializer::packInt(char* buffer, int val) {
#ifndef INTEL_X86
#error Not on Intel, serializing incorrect. Expecting little-endian.
#endif
	unsigned char * temp = (unsigned char *)&val;
	buffer[0] = temp[3];
	buffer[1] = temp[2];
	buffer[2] = temp[1];
	buffer[3] = temp[0];

	//4 bytes
	return 4;
}

std::string Serializer::unpackString(const char* buffer, int &p) {
	std::string result;

	int i = 0;
	while(buffer[i]) {
		result += buffer[i++];
	}

	//add one to 
	//account for null byte
	p += i + 1;

	return result;
}

int Serializer::packString(char* buffer, std::string str) {
#ifndef INTEL_X86
#error Not on Intel, serializing incorrect. Expecting little-endian.
#endif
	int numBytes = 0;
	for (char character : str) {
		buffer[numBytes++] = character;
	}

	//Null terminate
	buffer[numBytes++] = 0;

	//num bytes
	return numBytes;
}

char Serializer::unpackByte(const char* buffer, int &p) {
	//One byte
	p++;

	return buffer[0];
}

int Serializer::packByte(char* buffer, char byte) {
#ifndef INTEL_X86
#error Not on Intel, serializing incorrect. Expecting little-endian.
#endif

	//Yes...
	buffer[0] = byte;

	//One byte
	return 1;
}

std::map<GameObjectGlobalID, GameObject> Serializer::unpackMap(char *buffer, int &p) {
	// TODO: Handle unserialization of Player objects (which extend GameObjects)
	std::map<GameObjectGlobalID, GameObject> result;

	int map_len = Serializer::unpackInt(buffer, p);
	for (int i = 0; i < map_len; i++) {
		GameObjectGlobalID curr_id = Serializer::unpackInt(buffer, p);
		// TODO: re-enable when unserialize method for GameObject works on Client
		// GameObject curr_obj(buffer);
		// // a copy is made for the map
		// result[curr_id] = curr_obj;
	}
	return result;
}

int Serializer::packMap(
    char* buffer,
    std::map<GameObjectGlobalID, GameObject> objectMap
) {
	// TODO: Using C++ streams would make things much more elegant.
	int bytes_used = 0;
	int tmp_cnt;

	tmp_cnt = Serializer::packInt(buffer, objectMap.size());
	bytes_used += tmp_cnt;
	buffer += tmp_cnt;

	for(auto it = objectMap.begin(); it != objectMap.end(); ++it) {
		// pack GameObjectGlobalID
		tmp_cnt = Serializer::packInt(buffer, it->first);
		bytes_used += tmp_cnt;
		buffer += tmp_cnt;

		// pack GameObject
		// TODO: re-enable when serialize method for GameObject works on client
		//       (or when this part of the Serializer class is ported to server)
		// tmp_cnt = it->second.serialize(buffer);
		// bytes_used += tmp_cnt;
		// buffer += tmp_cnt;
	}
	return bytes_used;
}

