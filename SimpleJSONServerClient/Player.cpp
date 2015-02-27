#include "Player.hpp"

Player::Player(GameObjectGlobalID id) : GameObject(id) {
	(this->userControllable) = true;
	(this->score) = 0;
	(this->deleted) = false;
	(this->pitch) = 0;
	(this->roll) = 0;
	(this->yaw) = 0;
}

Player::Player(unsigned char *serial, int &size) {
	size += (this->deserialize(serial));
}

Player::~Player() {

}

Player::Player(const Player& other) : GameObject(other) {
	//copy Control control;
	(this->control) = KeyboardControl(other.control);
	(this->pitch) = other.pitch;
	(this->roll) = other.roll;
	(this->yaw) = other.yaw;
	(this->score) = other.score;
}

void Player::keyPressed(int key) {
	(this->control).regKeyPress(key);
}

void Player::setPRY(float pitch, float roll, float yaw) {
	(this->pitch) = pitch;
	(this->roll) = roll;
	(this->yaw) = yaw;
}

void Player::keyUnpressed(int key) {
	(this->control).regKeyUnpress(key);
}

//places serialized object in buffer and return the size as an int
int Player::serialize(unsigned char* buffer) {
	Serializer serializer;
	int next;
	next = serializer.packInt(buffer, (this->ID));
	if (this->physComp) {
		next += (this->physComp)->serialize(serializer, &(buffer[next]));
	}
	next += serializer.packBool(&buffer[next], visible);
	next += serializer.packBool(&buffer[next], renderable);
	next += control.serialize(serializer,&buffer[next]);
	next += serializer.packFloat(&buffer[next], pitch);
	next += serializer.packFloat(&buffer[next], roll);
	next += serializer.packFloat(&buffer[next], yaw);
	next += serializer.packInt(&buffer[next], score);
	return next;
}

//places serialized values into object return the size of the buffer read as an int 
int Player::deserialize(unsigned char* buffer) {
	Serializer serializer = Serializer();
	int next = 0;
	(this->ID) = serializer.unpackInt(buffer, next);
	(this->physComp) = std::make_shared<PhysicsObject>(serializer, &buffer[next],next);
	(this->visible) = serializer.unpackBool(&buffer[next], next);
	(this->renderable) = serializer.unpackBool(&buffer[next], next);
	(this->control) = KeyboardControl(serializer,&buffer[next],next);
	(this->pitch) = serializer.unpackFloat(&buffer[next], next);
	(this->roll) = serializer.unpackFloat(&buffer[next], next);
	(this->yaw) = serializer.unpackFloat(&buffer[next], next);
	(this->score) = serializer.unpackInt(&buffer[next], next);
	return next;
}

// bool* Player::getKeysPressed(){
// 	return control.getCurrentControls();
// }

bool Player::getUp() {
	return (this->control).getUp();
}

bool Player::getDown() {
	return (this->control).getDown();
}

bool Player::getLeft() {
	return (this->control).getLeft();
}

bool Player::getRight() {
	return (this->control).getRight();
}

int Player::getScore() const {
	return score;
}

void Player::modifyScore(int delta) {
	score += delta;
}

float Player::getPitch() {
	return (this->pitch);
}

float Player::getRoll() {
	return (this->roll);
}

float Player::getYaw() {
	return (this->yaw);
}

void Player::setControl(KeyboardControl control) {
	this->control = control;
}

KeyboardControl Player::getControl() {
	return (this->control);
}