#ifndef PLAYER_H
#define PLAYER_H

#include "Cube.hpp"
#include "GameObject.hpp"
#include "Controls.hpp"

class Player : public GameObject {
public:
	KeyboardControl control;
	float pitch;
	float roll;
	float yaw;
	int score;

	Player(float xPos, float yPos, float zPos);
	int deserialize(unsigned char* buffer);
};

#endif