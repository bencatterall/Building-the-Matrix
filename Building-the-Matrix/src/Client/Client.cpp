#include "../Serializer.hpp"
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
	Serializer serializer = Serializer();
	unsigned char *data = new unsigned char[15];
	data[0] = 'P';
	data[1] = 'R';
	data[2] = 'Y';
	int next = 3;
	next += serializer.packFloat(&data[next], pry.x);
	next += serializer.packFloat(&data[next], pry.y);
	next += serializer.packFloat(&data[next], pry.z);
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

bool Client::sendKeyPress(int key) {
	if (!(0 <= key && key < 256)) {
		std::cout << "Key " << (char)key << " (" << key << ") out of range for press\n";
		return false;
	}
	const unsigned char *data = constants.keyPressMessage(key);
	std::cout << "Key " << (char)key << " (" << key << ") pressed\n";
	bool a;
	(this->lock).lock();
	a = ((this->socket).sendSingle((this->server), (unsigned char*)data, 7 + 1 + sizeof(uint32_t) + 1));
	(this->lock).unlock();
	return a;
}

bool Client::sendKeyUnpress(int key) {
	if (!(0 <= key && key < 256)) {
		std::cout << "Key " << (char)key << " (" << key << ") out of range for unpress\n";
		return false;
	}
	const unsigned char	 *data = constants.keyUnpressMessage(key);
	std::cout << "Key " << (char)key << " (" << key << ") unpressed\n";
	bool a;
	(this->lock).lock();
	a = ((this->socket).sendSingle((this->server), (unsigned char*)data, 9 + 1 + sizeof(uint32_t) + 1));
	(this->lock).unlock();
	return a;
}

bool Client::sendLoginRequest() {
	const char *data = constants.loginMessage();
	return ((this->socket).sendSingle((this->server), (unsigned char*)data, 10));
}

bool Client::sendLogout() {
	const char *data = constants.logoutMessage();
	return ((this->socket).sendSingle((this->server), (unsigned char*)data, 10));
}

//attempts to send the bytestream given as an argument, and returns true if successful

bool Client::send(const char *data, int size) {
	return ((this->socket).sendSingle((this->server), (unsigned char*)data, size));
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
