#ifndef SENDER_H
#define SENDER_H

#include "SafeQueue.hpp"
#include "Address.hpp"
#include "Message.hpp"
#include "Socket.hpp"
#include "SafeMap.hpp"

class Sender {
	private:
		Socket serverSocket;
		SafeQueue<Message> toSend;
		bool cont;
	public:
		Sender();
		void setSocket(Socket *s);
		~Sender();
		Sender(const Sender& sender){} //don't want compiler generating this function as mutexes are uncopyable
		void sendUpdateMessage(Address client, std::map<GameObjectGlobalID, GameObject> message);
		void sendAck(Address client,std::string event);
		void run();
		void stop();
};
#endif