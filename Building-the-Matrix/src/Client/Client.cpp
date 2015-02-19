
/**
	Client.cpp
	Purpose: For game loop to access sending/receiving messages facilities

*/

#include "Client.hpp"
#include <thread>
#include <chrono>
#include <iostream>

Client::Client() : timeout(6) {}

//binds client to a socket and stores address of the server and its own Socket. An int is thrown if something goes wrong
void Client::setAddresses(Address client, Address server) {
	this->socket = Socket();
	if (!(this->socket).openSocket(client.getAddress(), client.getPort())) {
		std::cout << "failed to open client socket\n";
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
	std::cout << key << " pressed\n";
	bool a;
	(this->lock).lock();
	a = ((this->socket).sendSingle((this->server), data, 10));
	(this->lock).unlock();
	return a;
}

bool Client::sendKeyUnpress(char key) {
	const char *data = constants.keyUnpressMessage(key);
	std::cout << key << " unpressed\n";
	bool a;
	(this->lock).lock();
	a = ((this->socket).sendSingle((this->server), data, 12));
	(this->lock).unlock();
	return a;
}

bool Client::sendLoginRequest() {
	const char *data = constants.loginMessage();
	return ((this->socket).sendSingle((this->server), data, 10));
}

bool Client::sendLogout() {
	const char *data = constants.logoutMessage();
	return ((this->socket).sendSingle((this->server), data, 10));
}

//attempts to send the bytestream given as an argument, and returns true if succesful

bool Client::send(const char *data, int size) {
	return ((this->socket).sendSingle((this->server), data, size));
}

//returns number of bytes read from packet in the buffer - if this function returns < 0 it means there was no packet to read
//else it gives the number of bytes that were received
int Client::receive(char *data, int size) {
	Address sender;
	int bytes_read = (this->socket).receive(sender, (char *)data, size);
	if (bytes_read) {
		time_last_updated = std::chrono::system_clock::now();
	}
	return bytes_read;
}

std::chrono::time_point<std::chrono::system_clock> Client::getTimeLastUpdated() const {
	return time_last_updated;
}

bool Client::timedOut() const {
	return (std::chrono::system_clock::now() - time_last_updated) > timeout;
}
