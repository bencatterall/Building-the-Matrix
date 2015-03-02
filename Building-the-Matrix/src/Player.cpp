#include "GameObject.hpp"
#include "Cube.hpp"
#include "Chunk.hpp"
#include "Player.hpp"
#include "ObjectManager.hpp"
#include "LocationComponent.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

Player::Player(float xPos, float yPos, float zPos) :
GameObject(glm::vec3(xPos, yPos, zPos)) {
	headCube = std::make_shared<Cube>(glm::vec3(xPos, yPos + Chunk::getCubeSize(), zPos), (float)Chunk::getCubeSize() / 5.0f);
	vehicleCube = std::make_shared<Cube>(glm::vec3(xPos, yPos, zPos - 20.0f), (float)Chunk::getCubeSize());

	//Add to object mangager
	ObjectManager& objManager = ObjectManager::getInstance();
	objManager.addObject(headCube);
	//objManager.addObject(vehicleCube);
}

//Up is aways  (0.0, 1.0, 0.0)
void Player::setVehicleOrientation(glm::vec3 forwardVec) {
	glm::mat4 rotationMat = glm::orientation(forwardVec, glm::vec3(0.0f, 1.0f, 0.0f));
	vehicleCube->getLocationComponent()->setRotationMatrix(rotationMat);
}

void Player::setHeadOrientation(glm::vec3 PRY) {
	glm::mat4 rotationMatrix = glm::mat4(1.0f);

	//roll
	rotationMatrix = glm::rotate(rotationMatrix, PRY.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//yaw
	rotationMatrix = glm::rotate(rotationMatrix, PRY.z, glm::vec3(0.0f, 1.0f, 0.0f));

	//pitch
	rotationMatrix = glm::rotate(rotationMatrix, PRY.y, glm::vec3(1.0f, 0.0f, 0.0f));

	headCube->getLocationComponent()->setRotationMatrix(rotationMatrix);
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

	//Update orientation of the player
	setVehicleOrientation(physicsObject->getOrientation());
	setHeadOrientation(glm::vec3(this->roll, this->pitch, this->yaw));


	return next;
}