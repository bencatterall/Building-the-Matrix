#include "GameObject.hpp"
#include "Cube.hpp"
#include "Player.hpp"

#include <glm/glm.hpp>

Player::Player(float xPos, float yPos, float zPos) : 
	GameObject(glm::vec3(xPos, yPos, zPos)) { 

}

//places serialized values into object return the size of the buffer read as an int 
int Player::deserialize(unsigned char* buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	this->setGlobalID(serializer.unpackInt(buffer, next));
	(this->physicsObject) = std::make_shared<PhysicsObject>(&buffer[next], next);
	this->setVisible(serializer.unpackBool(&buffer[next], next));
	this->setRenderable(serializer.unpackBool(&buffer[next], next));
	(this->control) = KeyboardControl(serializer, &buffer[next], next);
	(this->pitch) = serializer.unpackFloat(&buffer[next], next);
	(this->roll) = serializer.unpackFloat(&buffer[next], next);
	(this->yaw) = serializer.unpackFloat(&buffer[next], next);
	(this->score) = serializer.unpackInt(&buffer[next], next);
	(this->locationComponent) = (this->physicsObject)->getLocationComponent();
	return next;
}