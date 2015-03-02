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
	headCube = std::make_shared<Cube>(glm::vec3(xPos, yPos, zPos), (float)Chunk::getCubeSize() / 2.0f);
	vehicleCube = std::make_shared<Cube>(glm::vec3(xPos, yPos - 1.5*Chunk::getCubeSize(), zPos), (float)Chunk::getCubeSize());

	//Add to object manager
	ObjectManager& objManager = ObjectManager::getInstance();
	objManager.addObject(headCube);
	objManager.addObject(vehicleCube);
}

//Up is aways  (0.0, 1.0, 0.0)
void Player::setVehicleOrientation(glm::vec3 forwardVec) {
	//glm::mat4 rotationMat = glm::orientation(forwardVec, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rotationMat = glm::orientation(forwardVec, glm::vec3(0.0f, 0.0f, -1.0f));
	(this->getLocationComponent())->setRotationMatrix(rotationMat);
	vehicleCube->getLocationComponent()->setRotationMatrix(rotationMat);
}

//NOTE: INVERTED WHEN GOING IN THE +Z DIRECTION - THINK IT ASSUMES FORWARD FACING DIRECTION TO BE CONSTANT
void Player::setHeadOrientation(glm::vec3 PRY) {
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	
	//roll
	rotationMatrix = glm::rotate(rotationMatrix, PRY.x, glm::vec3(0.0f, 0.0f, 1.0f));

	//yaw
	rotationMatrix = glm::rotate(rotationMatrix, PRY.z, glm::vec3(0.0f, 1.0f, 0.0f));

	//pitch
	rotationMatrix = glm::rotate(rotationMatrix, PRY.y, glm::vec3(1.0f, 0.0f, 0.0f));

	headCube->getLocationComponent()->setRotationMatrix(rotationMatrix * ((this->getLocationComponent()->getRotationMatrix())));
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

	//update position of head and body
	glm::vec3 newPos = this->getLocationComponent()->getPosition();
	headCube->getLocationComponent()->setPosition(glm::vec3(newPos.x, newPos.y, newPos.z));
	vehicleCube->getLocationComponent()->setPosition(glm::vec3(newPos.x, newPos.y - 1.5*Chunk::getCubeSize(), newPos.z));
	//Update orientation of the player
	setVehicleOrientation(physicsObject->getOrientation());
	setHeadOrientation(glm::vec3(this->roll, this->pitch, this->yaw));


	return next;
}