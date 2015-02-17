#ifdef MESSAGE_CONSTANTS_H
#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.hpp"
#include "MessageConstants.hpp"
#include "Address.hpp"
#include "../Common.hpp"

#include <chrono>

class Client {
private:
	Socket socket;
	Address server;
	MessageConstants constants;
    std::chrono::time_point<std::chrono::system_clock> time_last_updated;
public:
	Client() {}
	Client(Address client, Address server);
	~Client();
	bool sendKeyPress(char key);
	bool sendKeyUnpress(char key);
	bool sendLoginRequest(GameObjectGlobalID id);
	bool sendLogout();
	bool send(const char *data);
	int receive(const char *data);
    std::chrono::time_point<std::chrono::system_clock> getTimeLastUpdated() const;
};

#endif
#endif
