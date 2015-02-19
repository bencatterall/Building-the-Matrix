#include "Controls.hpp"

Control::Control() {}

Control::~Control() {}

void Control::regKeyPress(char key) {
	if (key == 's') {
		(this->S_HELD) = true;
	}
	if (key == 'c') {
		(this->C_HELD) = true;
	}
	if (key == 'r') {
		(this->R_HELD) = true;
	}
	if (key == 'l') {
		(this->L_HELD) = true;
	}
}

void Control::regKeyUnpress(char key) {
	if (key == 's') {
		(this->S_HELD) = false;
	}
	if (key == 'c') {
		(this->C_HELD) = false;
	}
	if (key == 'r') {
		(this->R_HELD) = false;
	}
	if (key == 'l') {
		(this->L_HELD) = false;
	}
}

bool *Control::getCurrentControls() {
	bool *arr = new bool[ S_HELD, C_HELD, R_HELD, L_HELD ];
	return arr;
}