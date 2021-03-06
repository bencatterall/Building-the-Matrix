#ifndef SOCKET_H
#define SOCKET_H

#include "Address.hpp"
// allow server to be multi-platform - TODO is this needed or will we just run it on Windows?
#ifndef PLATFORM
#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3
#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__) // _WIN32
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif // __APPLE__
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#pragma comment( lib, "wsock32.lib" )
#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
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