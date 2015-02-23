#include "MessageConstants.hpp"
#include <iostream>

char *MessageConstants::login = "LOGIN\0";
char *MessageConstants::logout = "LOGOUT\0";
char *MessageConstants::pressed = "PRESSED";
char *MessageConstants::unpressed = "UNPRESSED";

MessageConstants::MessageConstants() {}
MessageConstants::~MessageConstants() {}

const char *MessageConstants::loginMessage() {
	return login;
}

const char *MessageConstants::logoutMessage() {
	return logout;
}

const char *MessageConstants::keyPressMessage(char key) {
	char *m = new char[10];
	for (unsigned int i = 0; i < strlen(pressed); i++){
		m[i] = pressed[i];
	}
	m[8] = key;
	m[9] = '\0';
	return m;
}

const char *MessageConstants::keyUnpressMessage(char key) {
	char *m = new char[12];
	for (unsigned int i = 0; i < strlen(unpressed); i++){
		m[i] = unpressed[i];
	}
	m[10] = key;
	m[11] = '\0';
	return m;
}