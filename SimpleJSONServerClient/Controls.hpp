#ifndef CONTROL_H
#define CONTROL_H
#include <vector>
#include <string>
#include "Serializer.hpp"

class Serializer;

class KeyboardControl {
	private:
		std::vector<bool> keys_held;
	public:
		KeyboardControl(Serializer serializer, unsigned char *buffer, int &size);
		KeyboardControl();
		KeyboardControl(bool up,bool down,bool left,bool right);
		~KeyboardControl();
		KeyboardControl(const KeyboardControl& other);
		void regKeyPress(int key);
		void regKeyUnpress(int key);
		std::vector<bool> getCurrentControls();
		int serialize(Serializer serializer, unsigned char *buffer);
		int deserialize(Serializer serializer, unsigned char *buffer);
		bool getUp() const;
		bool getDown() const;
		bool getLeft() const;
		bool getRight() const;
		bool getKey(unsigned int) const;
};
#endif