#ifndef ADDRESS_H
#define ADDRESS_H

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif

#include <string>

class Address {
private:
	unsigned int address;
	unsigned short port;

public:
	Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short port);
	Address(unsigned int address, unsigned short port);
	Address(std::string ipAddr, unsigned short port);
	Address();
	~Address();
	unsigned int getAddress() const;
	std::string getHBOAddress() const;
	unsigned short getPort() const;
	unsigned short getHBOPort() const;
};
#endif
