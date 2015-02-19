#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.hpp"
#include "MessageConstants.hpp"
#include "Address.hpp"
#include "../Common.hpp"
#include <mutex>

class Client {
private:
	Socket socket;
	Address server;
	std::mutex lock;
	MessageConstants constants;
public:
	Client();
	void setAddresses(Address client, Address server);
	~Client();
	bool sendKeyPress(char key);
	bool sendKeyUnpress(char key);
	bool sendLoginRequest();
	bool sendLogout();
	bool send(char *data);
	int receive(char *data,int size);
};

#endif

