#include "MessageConstants.hpp"
#include "../Serializer.hpp"
#include <iostream>
#include <cstring>

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

const unsigned char *MessageConstants::keyPressMessage(int key) {
	unsigned char *m = new unsigned char[7 + 1 + sizeof(uint32_t) + 1];
	memcpy(m, pressed, strlen(pressed));
	pressed[7] = ' ';
	int next = 8;
	next += Serializer::packInt(&m[8], key);
	m[next] = '\0';
	return m;
}

const unsigned char *MessageConstants::keyUnpressMessage(int key) {
	unsigned char *m = new unsigned char[9 + 1 + sizeof(uint32_t) + 1];
	memcpy(m, unpressed, strlen(unpressed));
	unpressed[9] = ' ';
	int next = 10;
	next += Serializer::packInt(&m[10], key);
	m[next] = '\0';
	return m;
}