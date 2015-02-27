#include "Controls.hpp"
// for keyboard constants
#include "GLFW/glfw3.h"
#include <bitset>

KeyboardControl::KeyboardControl(): keys_held(512, false) {}

KeyboardControl::~KeyboardControl() {}

KeyboardControl::KeyboardControl(const KeyboardControl& other) {
	(this->keys_held) = other.keys_held;
}

KeyboardControl::KeyboardControl(Serializer serializer, unsigned char *buffer, int &next) {
	next += (this->deserialize(serializer, buffer));
}

void KeyboardControl::regKeyPress(int key) {
	keys_held[key] = true;
}

void KeyboardControl::regKeyUnpress(int key) {
	keys_held[key] = false;
}

std::vector<bool> KeyboardControl::getCurrentControls() {
	return keys_held;
}

int KeyboardControl::serialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	for (int i = 0; i < keys_held.size(); i += sizeof(uint32_t)) {
		std::bitset<sizeof(uint32_t)> bset;
		for (int j = 0; j < sizeof(uint32_t); j++) {
			if (keys_held[i + j]) {
				bset.set(j);
			}
		}
		next += serializer.packInt(&buffer[next], bset.to_ulong());
	}
	return next;
}

int KeyboardControl::deserialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	for (int i = 0; i < keys_held.size(); i += sizeof(uint32_t)) {
		int curr_key = serializer.unpackBool(&buffer[next], next);
		std::bitset<sizeof(uint32_t)> bset(curr_key);
		for (int j = 0; j < sizeof(uint32_t); j++) {
			keys_held[i + j] = bset.test(j);
		}
	}
	return next;
}

bool KeyboardControl::getUp() {
	return keys_held[GLFW_KEY_UP];
}

bool KeyboardControl::getDown() {
	return keys_held[GLFW_KEY_DOWN];
}

bool KeyboardControl::getLeft() {
	return keys_held[GLFW_KEY_LEFT];
}

bool KeyboardControl::getRight() {
	return keys_held[GLFW_KEY_RIGHT];
}