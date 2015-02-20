#include "Controls.hpp"

Control::Control() {}

Control::~Control() {}

Control::Control(const Control& other) {
	(this->U_HELD) = other.U_HELD;
	(this->D_HELD) = other.D_HELD;
	(this->L_HELD) = other.L_HELD;
	(this->R_HELD) = other.R_HELD;
}

void Control::regKeyPress(char key) {
	if (key == 'u') {
		(this->U_HELD) = true;
	}
	if (key == 'd') {
		(this->D_HELD) = true;
	}
	if (key == 'r') {
		(this->R_HELD) = true;
	}
	if (key == 'l') {
		(this->L_HELD) = true;
	}
}

void Control::regKeyUnpress(char key) {
	if (key == 'u') {
		(this->U_HELD) = false;
	}
	if (key == 'd') {
		(this->D_HELD) = false;
	}
	if (key == 'r') {
		(this->R_HELD) = false;
	}
	if (key == 'l') {
		(this->L_HELD) = false;
	}
}

bool *Control::getCurrentControls() {
	bool *arr = new bool[ U_HELD, D_HELD, R_HELD, L_HELD ];
	return arr;
}