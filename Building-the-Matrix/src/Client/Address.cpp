/**
	Address.cpp
	Purpose: Stores an address and a port in network byte order to be used by a socket

*/

#include "Address.hpp"
#include <sstream>

//Create Address from IP given in a.b.c.d format and port
Address::Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short port){
	//store in network byte order as this is what the socket send method will require
	(this->address) = htonl((a << 24) | (b << 16) | (c << 8) | d);
	(this->port) = htons(port);
}

//Create address from integer IP address format and port
Address::Address(unsigned int address, unsigned short port){
	//assume already given in network byte order
	(this->address) = address;
	(this->port) = port;
}

Address::Address(std::string ipAddr, unsigned short port) {

	//convert a string to a network byte-order array
	//TODO: Put in it's own function
	size_t pos = 0;
	int acc = 0;
	int IPValues[4];
	while ((pos = ipAddr.find('.')) != std::string::npos) {
		IPValues[acc] = atoi((ipAddr.substr(0, pos)).c_str());
		ipAddr = ipAddr.substr(pos + 1, (ipAddr.length() - (pos + 1)));
		acc++;
	}
	IPValues[3] = atoi(ipAddr.c_str());

	Address(IPValues[0], IPValues[1], IPValues[2], IPValues[3], port);
}

//empty constructor
Address::Address(){}

//no resources to deallocate
Address::~Address(){}

//returns private member address
unsigned int Address::getAddress() const {
	return (this->address);
}

//returns private member port
unsigned short Address::getPort() const{
	return (this->port);
}

std::string Address::getHBOAddress() const {
	int a = 0xFF & (this->address);
	int b = (0xFF00 & (this->address)) >> 8;
	int c = (0xFF0000 & (this->address)) >> 16;
	int d = (0xFF000000 & (this->address)) >> 24;
	std::stringstream ss;
	ss << a << '.' << b << '.' << c << '.' << d;
	return ss.str();
}

unsigned short Address::getHBOPort() const {
	return ntohs(this->port);
}
