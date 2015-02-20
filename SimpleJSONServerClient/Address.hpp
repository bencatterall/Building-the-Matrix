#ifndef ADDRESS_H
#define ADDRESS_H

#include <string>
#include "../Common.hpp"

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

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
		std::string getHBOAddress() const;
		unsigned short getPort() const;
		unsigned short getHBOPort() const;
};
#endif