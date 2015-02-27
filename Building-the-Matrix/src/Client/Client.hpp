#ifndef CLIENT_H
#define CLIENT_H

#include "Socket.hpp"
#include "MessageConstants.hpp"
#include "Address.hpp"
//#include "../Common.hpp"
#include <mutex>
#include <glm/glm.hpp>
#include <chrono>

class Client {
private:
	Socket socket;
	Address server;
	std::mutex lock;
	MessageConstants constants;
    std::chrono::time_point<std::chrono::system_clock> time_last_updated;
public:
	const std::chrono::seconds timeout;
	Client();
	void setAddresses(Address client, Address server);
	~Client();
	bool sendKeyPress(int key);
	bool sendKeyUnpress(int key);
	bool sendPitchRollYaw(glm::vec3 pry);
	bool sendLoginRequest();
	bool sendLogout();
	bool send(const char *data, int size);
	int receive(unsigned char *data,int size);
    std::chrono::time_point<std::chrono::system_clock> getTimeLastUpdated() const;
    bool timedOut() const;
};

#endif