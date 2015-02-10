#ifndef ADDRESS_H
#define ADDRESS_H

#include <winsock2.h>

class Address {
private:
	unsigned int address;
	unsigned short port;

public:
	Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short port);
	Address(unsigned int address, unsigned short port);
	Address();
	~Address();
	unsigned int getAddress() const;
	unsigned int getHBOAddress() const;
	unsigned short getPort() const;
	unsigned short getHBOPort() const;
};
#endif