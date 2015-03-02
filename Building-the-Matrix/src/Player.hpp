#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "Cube.hpp"
#include "GameObject.hpp"
#include "Controls.hpp"

class Player : public GameObject {
	std::shared_ptr<Cube> headCube;
	std::shared_ptr<Cube> vehicleCube;
public:
	KeyboardControl control;
	float pitch;
	float roll;
	float yaw;
	int score;

	Player(float xPos, float yPos, float zPos);

	//Pitch, roll yaw
	void Player::setHeadOrientation(glm::vec3 PRY);
	void Player::setVehicleOrientation(glm::vec3 forwardVec);
	int deserialize(unsigned char* buffer);
};

#endif