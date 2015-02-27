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
	unsigned char *m = new unsigned char[7 + sizeof(uint32_t) + 1];
	memcpy(m, pressed, strlen(pressed));
	int next = 7;
	next += Serializer::packInt(&m[7], key);
	m[next] = '\0';
	return m;
}

const unsigned char *MessageConstants::keyUnpressMessage(int key) {
	unsigned char *m = new unsigned char[9 + sizeof(uint32_t) + 1];
	memcpy(m, unpressed, strlen(unpressed));
	int next = 9;
	next += Serializer::packInt(&m[9], key);
	m[next] = '\0';
	return m;
}