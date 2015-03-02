#ifndef MESSAGE_H
#define MESSAGE_H

#include "Address.hpp"
#include "SafeMap.hpp"
//THIS IS WHERE JSON WILL NEED TO CONVERT THE MAPS WE WANT TO SEND INTO STRING

class Message {
	private:
		Address client;
		unsigned char *message;
		int messageSize;
	public:
		Message(Address client, std::map<GameObjectGlobalID, GameObject> message);
		Message(Address client, std::string event);
		Message(Address client, unsigned char *message, int length);
		~Message();
		Address getClient();
		unsigned char* getMessage();
		std::map<GameObjectGlobalID, GameObject> parseMessage();
		int getMessageSize();
};
#endif