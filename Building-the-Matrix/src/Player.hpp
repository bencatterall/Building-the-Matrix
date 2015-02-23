#ifndef PLAYER_H
#define PLAYER_H

#include "Cube.hpp"
#include "Controls.hpp"

class Player : public Cube {

public:
	KeyboardControl control;
	float pitch;
	float roll;
	float yaw;
	int score;

	//BEN WHAT ARE THESE???
	Player(float xPos, float yPos, float zPos);
	int deserialize(unsigned char* buffer);
};

#endif