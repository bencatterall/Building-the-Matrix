#ifdef MESSAGE_CONSTANTS_H
/**
	Client.cpp
	Purpose: For game loop to access sending/receiving messages facilities

*/

#include "Client.hpp"
#include <thread>
#include <chrono>
#include <iostream>

//binds client to a socket and stores address of the server and its own Socket. An int is thrown if something goes wrong
Client::Client(Address client, Address server) {
	this->socket = Socket();
	if (!(this->socket).openSocket(client.getAddress(), client.getPort())) {
		throw int(1);
	}
	this->server = server;
}

//on client deletion, cleans up used resources
Client::~Client() {
	if (!(this->sendLogout())) {
		std::cerr << "Failed to send message";
	}
}

bool Client::sendKeyPress(char key) {
	const char *data = constants.keyPressMessage(key);
	return ((this->socket).sendSingle((this->server), data, sizeof(data)));
}

bool Client::sendKeyUnpress(char key) {
	const char *data = constants.keyUnpressMessage(key);
	return ((this->socket).sendSingle((this->server), data, sizeof(data)));
}

bool Client::sendLoginRequest(GameObjectGlobalID id) {
	const char *data = constants.loginMessage(id);
	return ((this->socket).sendSingle((this->server), data, sizeof(data)));
}

bool Client::sendLogout() {
	const char *data = constants.logoutMessage();
	return ((this->socket).sendSingle((this->server), data, sizeof(data)));
}

//attempts to send the bytestream given as an argument, and returns true if succesful
bool Client::send(char *data) {
	return ((this->socket).sendSingle((this->server), data, sizeof(data)));
}

//returns number of bytes read from packet in the buffer - if this function returns < 0 it means there was no packet to read
//else it gives the number of bytes that were received
int Client::receive(char *data) {
	Address sender;
	int bytes_read = (this->socket).receive(sender, (char *)data, sizeof(data));
	return bytes_read;
}
#endif
