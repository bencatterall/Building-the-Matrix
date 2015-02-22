#include "Player.hpp"

Player::Player(GameObjectGlobalID id) {
	(this->ID) = id;
	(this->userControllable) = true;
	(this->score) = 0;
	(this->deleted) = false;
}

Player::~Player() {

}

Player::Player(const Player& other) : GameObject(other) {
	//copy Control control;
	(this->control) = Control(other.control);
	(this->pitch) = other.pitch;
	(this->roll) = other.roll;
	(this->yaw) = other.yaw;
	(this->score) = other.score;
}

void Player::keyPressed(char key) {
	(this->control).regKeyPress(key);
}

void Player::setPRY(float pitch, float roll, float yaw) {
	(this->pitch) = pitch;
	(this->roll) = roll;
	(this->yaw) = yaw;
}

void Player::keyUnpressed(char key) {
	(this->control).regKeyUnpress(key);
}

bool* Player::getKeysPressed(){
	return control.getCurrentControls();
}
