#include "Sender.hpp"
#include "Player.hpp"
#include <iostream>

Sender::Sender() {
	(this->cont) = true;
}

Sender::~Sender() {}

void Sender::setSocket(Socket *s) {
	(this->serverSocket) = *s;
}

void Sender::sendUpdateMessage(Address client, std::map<GameObjectGlobalID, GameObject> message) {
	unsigned char *m = new unsigned char[1024];
	int size = 0;
	for (std::pair<GameObjectGlobalID, GameObject> go : message) {
		if (go.second.userControllable){
			//SERIALIZE AS A PLAYER OBJECT
			std::cout << "serialising a player object\n";
			m[size] = 'P';
			size++;
			Player *p = (Player *)&(go.second);
			int psize;
			psize = (p->serialize(&m[size]));
			std::cout << "player object size = " << psize << "\n";
			size += psize;
		}
		else {
			std::cout << "serialising a game object\n";
			m[size] = 'O';
			size++;
			int gosize;
			gosize = go.second.serialize(&m[size]);
			std::cout << "game object size = " << gosize << "\n";
			size += gosize;
		}
	}
	std::cout << "total map size = " << size << "\n";
	
	Message mess = Message(client,m,size);
	//TEMP
	int size2= 0;
	Player deserialized = Player(&m[1],size2);
	std::cout << "deserialized " << size2 << " bytes\n";
	(this->toSend).pushToEnd(mess);
}

void Sender::sendAck(Address client, std::string event) {
	Message m = Message(client, event);
	(this->toSend).pushToEnd(m);
}

void Sender::sendMessage(Address client, unsigned char *message, int length){
	Message m = Message(client, message, length);
	(this->toSend).pushToEnd(m);
}

//we only want one thread accessing send from the socket
void Sender::run() {
	while (this->cont) {
		if (!(this->toSend).isEmpty()) {
			Message m = (this->toSend).popFromFront();
			if (!((this->serverSocket).sendSingle(m.getClient(), (const char *)m.getMessage(), m.getMessageSize()))) {
				std::cout << "Failed to send message\n";
			}
		}
	}
}

void Sender::stop() {
	std::cout << "Called stop on sender";
	(this->cont) = false;
}