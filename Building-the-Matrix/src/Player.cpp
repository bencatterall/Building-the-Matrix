#include "GameObject.hpp"
#include "Cube.hpp"
#include "Player.hpp"

#include <glm/glm.hpp>

Player::Player(float xPos, float yPos, float zPos) : 
	Cube(glm::vec3(xPos, yPos, zPos)) { 

}