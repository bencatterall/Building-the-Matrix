#ifdef MESSAGE_CONSTANTS_H
#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.hpp"
#include "MessageConstants.hpp"
#include "Address.hpp"
#include "../Common.hpp"

class Client {
private:
	Socket socket;
	Address server;
	MessageConstants constants;
public:
	Client() {}
	Client(Address client, Address server);
	~Client();
	bool sendKeyPress(char key);
	bool sendKeyUnpress(char key);
	bool sendLoginRequest(GameObjectGlobalID id);
	bool sendLogout();
	bool send(char *data);
	int receive(char *data);
};

#endif
#endif
