#ifndef CONTROL_H
#define CONTROL_H
#include <string>
#include "Serializer.hpp"

class Serializer;

class KeyboardControl {
private:
	//up is down -> accelerate
	bool U_HELD;
	//down is down -> brake;
	bool D_HELD;
	//right is down -> turn right
	bool R_HELD;
	//left is down -> turn left
	bool L_HELD;
public:
	KeyboardControl(Serializer serializer, unsigned char *buffer, int &size);
	KeyboardControl();
	~KeyboardControl();
	KeyboardControl(const KeyboardControl& other);
	void regKeyPress(char key);
	void regKeyUnpress(char key);
	bool *getCurrentControls();
	int serialize(Serializer serializer, unsigned char *buffer);
	int deserialize(Serializer serializer, unsigned char *buffer);
};
#endif