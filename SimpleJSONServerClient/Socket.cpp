/**
	Socket.cpp
	Purpose: Abstraction for socket that sits between application and I/O in the OS - used to send and receive
	messages at the socket's bound address and port

*/

#include "Socket.hpp"
#include <iostream>

//Setup only required for Windows sockets
Socket::Socket(){
	#if PLATFORM == PLATFORM_WINDOWS
		WSADATA WsaData;
		//create socket with newest 2.2 specification
		if (!WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR) {
			std::cerr << "unable to initialize Windows sockets layer\n";
			exit(EXIT_FAILURE);
		}
	#endif
}

//cleanup also only required for Windows sockets
Socket::~Socket(){
	closeSocket();
	#if PLATFORM == PLATFORM_WINDOWS
		WSACleanup();
	#endif
}

//create a socket and bind it to the specified port
bool Socket::openSocket(int addr, unsigned short port) {
	//We create a socket that uses UDP for best real-time server/client performance,
	//and receive a handle to it
	(this->socketHandle) = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//something went wrong
	if ((this->socketHandle) <= 0)
	{
		std::cerr << "unable to create a socket\n";
		return false;
	}

	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = addr;
	//port must be provided in network byte order
	address.sin_port = port;

	//bind the port to the socket - this is the port clients should send their packets to
	if (bind((this->socketHandle), (const sockaddr*)&address, sizeof(sockaddr_in)) < 0)
	{
		//something went wrong
		std::cerr << "unable to bind socket to port " << port << "\n";
		return false;
	}

	//set socket to be non-blocking - we want to send/receive messages in real-time
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
		int nonBlocking = 1;
		if (fcntl((this->socketHandle), F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
			std::cerr << "failed to set socket to non-blocking\n";
			return false;
		}

	#elif PLATFORM == PLATFORM_WINDOWS
		DWORD nonBlocking = 1;
		if (ioctlsocket((this->socketHandle), FIONBIO, &nonBlocking) != 0) {
			std::cerr << "failed to set socket to non-blocking\n";
			return false;
		}
	#endif

	return true;
}

bool Socket::sendSingle(const Address &dest, const char *data, int size) {
	sockaddr_in destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = (dest.getAddress());
	destAddr.sin_port = (dest.getPort());
	int sent_bytes = sendto((this->getHandle()), (const char*)data, size, 0, (sockaddr*)&destAddr, sizeof(sockaddr_in));

	if (sent_bytes != size) {
		std::cerr << "failed to send packet with all bytes of required data\n";
		return false;
	}

	return true;
}

int Socket::receive(Address &sender, char *data, int size){	
	#if PLATFORM == PLATFORM_WINDOWS
		typedef int socklen_t;
	#endif

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);

	//get next message from server socket's buffer
	int bytes = recvfrom(this->socketHandle, data, size, 0, (sockaddr*)&from, &fromLength);
	sender = Address(from.sin_addr.s_addr, from.sin_port);

	return bytes;
}

void Socket::closeSocket() {
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
			close(this->socketHandle);
	#elif PLATFORM == PLATFORM_WINDOWS
		closesocket(this->socketHandle);
	#endif
}

short Socket::getHandle() {
	return this->socketHandle;
}


