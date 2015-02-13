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
	/* will add back in when we're accessing the server
	bool confirmed = false;
	int attempts = 0;
	char data[] = "LOGIN";
	if (!(this->send(data))) {
		std::cerr << "Failed to send message";
		throw int(3);
	}
	attempts++;
	while (!confirmed) {
		//resend login request if server doesn't reply within a second
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Address sender;
		unsigned char buffer[256];
		int bytes_read = this->receive((char *)buffer);
		if (bytes_read >= 0) {
			std::string recmessage((char *)buffer);
			std::cout << "Client received message: " << recmessage << "\n";
			confirmed = true;
		}
		else {
			if (attempts == 5) {
				throw int(2);
			}
			if (!(this->send(data))) {
				std::cerr << "Failed to send message";
				throw int(3);
			}
			attempts++;
		}
	}
	*/
}

//on client deletion, cleans up used resources
Client::~Client() {
	const char data[] = "LOGOUT";
	if (!(this->send((char *)data))) {
		std::cerr << "Failed to send message";
	}
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