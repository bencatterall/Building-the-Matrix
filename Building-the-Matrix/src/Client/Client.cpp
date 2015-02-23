
///
/// Client.cpp
/// Purpose: For game loop to access sending/receiving messages facilities
///

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

bool Client::sendPitchRollYaw(glm::vec3 pry) {
	bool a;
	unsigned char *p = (unsigned char *)&pry.x;
	unsigned char *r = (unsigned char *)&pry.y;
	unsigned char *y = (unsigned char *)&pry.z;
	char *data = new char[15];
	data[0] = 'P';
	data[1] = 'R';
	data[2] = 'Y';
	data[3] = p[0];
	data[4] = p[1];
	data[5] = p[2];
	data[6] = p[3];
	data[7] = r[0];
	data[8] = r[1];
	data[9] = r[2];
	data[10] = r[3];
	data[11] = y[0];
	data[12] = y[1];
	data[13] = y[2];
	data[14] = y[3];
	(this->lock).lock();
	a = ((this->socket).sendSingle((this->server), data, 15));
	(this->lock).unlock();
	return a;
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

//attempts to send the bytestream given as an argument, and returns true if successful

bool Client::send(const char *data, int size) {
	return ((this->socket).sendSingle((this->server), data, size));
}

//returns number of bytes read from packet in the buffer - if this function returns < 0 it means there was no packet to read
//else it gives the number of bytes that were received
int Client::receive(unsigned char *data, int size) {
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
