#ifndef PLAYER_H
#define PLAYER_H

#include <memory>

#include "Cube.hpp"
#include "GameObject.hpp"
#include "Controls.hpp"
#include "TextureAtlas.hpp"

class Player : public GameObject {
	std::shared_ptr<Cube> headCube;
	std::shared_ptr<Cube> vehicleCube;

	void setTileTexture(float xPos, float yPos, float zPos, int* numVertices, std::string tileName, std::shared_ptr<TextureAtlas> texture,
		const glm::vec3(&vertexData)[6], const GLfloat(&texData)[6 * 2], std::vector<GLfloat>& vertData, std::vector<GLfloat>& texCoords, float sf);

public:
	KeyboardControl control;
	float pitch;
	float roll;
	float yaw;
	int score;

	Player(float xPos, float yPos, float zPos);

	//Pitch, roll yaw
	void setHeadOrientation(glm::vec3 PRY);
	void setVehicleOrientation(glm::vec3 forwardVec);
	int deserialize(unsigned char* buffer);
};

#endif