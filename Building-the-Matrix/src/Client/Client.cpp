/**
	Client.cpp
	Purpose: For game loop to access sending/receiving messages facilities

*/

#include "Client.hpp"

//binds client to a socket and stores address of the server and its own Socket
Client::Client(Address client, Address server) {
	this->socket = Socket();
	//TODO: what to do if this fails?
	(this->socket).openSocket(client.getAddress(),client.getPort());
	this->server = server;
}

//on client deletion, cleans up used resources
Client::~Client() {
	(this->socket).~Socket();
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