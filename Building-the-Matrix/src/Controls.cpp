#include "Controls.hpp"

KeyboardControl::KeyboardControl() {}

KeyboardControl::~KeyboardControl() {}

KeyboardControl::KeyboardControl(const KeyboardControl& other) {
	(this->U_HELD) = other.U_HELD;
	(this->D_HELD) = other.D_HELD;
	(this->L_HELD) = other.L_HELD;
	(this->R_HELD) = other.R_HELD;
}

KeyboardControl::KeyboardControl(Serializer serializer, unsigned char *buffer, int &next) {
	next += (this->deserialize(serializer, buffer));
}

void KeyboardControl::regKeyPress(char key) {
	if (key == 'u') {
		(this->U_HELD) = true;
	}
	if (key == 'd') {
		(this->D_HELD) = true;
	}
	if (key == 'r') {
		(this->R_HELD) = true;
	}
	if (key == 'l') {
		(this->L_HELD) = true;
	}
}

void KeyboardControl::regKeyUnpress(char key) {
	if (key == 'u') {
		(this->U_HELD) = false;
	}
	if (key == 'd') {
		(this->D_HELD) = false;
	}
	if (key == 'r') {
		(this->R_HELD) = false;
	}
	if (key == 'l') {
		(this->L_HELD) = false;
	}
}

bool *KeyboardControl::getCurrentControls() {
	bool *arr = new bool[U_HELD, D_HELD, R_HELD, L_HELD];
	return arr;
}

int KeyboardControl::serialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	next += serializer.packBool(&buffer[next], U_HELD);
	next += serializer.packBool(&buffer[next], D_HELD);
	next += serializer.packBool(&buffer[next], R_HELD);
	next += serializer.packBool(&buffer[next], L_HELD);
	return next;
}

int KeyboardControl::deserialize(Serializer serializer, unsigned char *buffer) {
	int next = 0;
	(this->U_HELD) = serializer.unpackBool(&buffer[next], next);
	(this->D_HELD) = serializer.unpackBool(&buffer[next], next);
	(this->R_HELD) = serializer.unpackBool(&buffer[next], next);
	(this->L_HELD) = serializer.unpackBool(&buffer[next], next);
	return next;
}