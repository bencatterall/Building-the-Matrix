#ifndef BSDSOCKET_H
#define BSDSOCKET_H

#include "Address.hpp"
#define PLATFORM_WINDOWS  1
#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#endif
#if PLATFORM == PLATFORM_WINDOWS
#pragma comment( lib, "wsock32.lib" )
#endif

class Socket {
private:
	short socketHandle;

public:
	Socket();
	~Socket();
	bool openSocket(int address, unsigned short port);
	void closeSocket();
	bool sendSingle(const Address &dest, const char *data, int size);
	int receive(Address &sender, char *data, int size);
	short getHandle();
};
#endif