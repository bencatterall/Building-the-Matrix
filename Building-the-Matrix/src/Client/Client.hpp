#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.hpp"
#include "Address.hpp"

class Client {
private:
	Socket socket;
	Address server;

public:
	Client(Address client, Address server);
	~Client();
	bool send(char *data);
	int receive(char *data);
};

#endif

