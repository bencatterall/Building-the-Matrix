#include "Sender.hpp"
#include "Player.hpp"
#include <iostream>
#include <thread>

Sender::Sender() {
	(this->cont) = true;
}

Sender::~Sender() {}

void Sender::setSocket(Socket *s) {
	(this->serverSocket) = *s;
}

void Sender::sendUpdateMessage(std::vector <Address> addresses, std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> message) {
	unsigned char *m = new unsigned char[1024];
	int size = 0;
	for (std::pair<GameObjectGlobalID, std::shared_ptr<GameObject>> go : message) {
		bool deleted = go.second->deleted;
		if (go.second->userControllable){
			//SERIALIZE AS A PLAYER OBJECT
			//std::cout << "serialising a player object\n";
			m[size] = 'P';
			if (deleted) {
				m[size + 1] = 'D';
			}
			else {
				m[size + 1] = 'K';
			}
			size += 2;

			std::shared_ptr<Player> p = std::dynamic_pointer_cast<Player>(go.second);
			glm::vec3 pos = p->locComp->getPosition();
		//	std::cout << "BObject position:" << pos.x << " " << pos.y << " " << pos.z << std::endl;
			int psize;
			psize = (p->serialize(&m[size]));
			//std::cout << "player object size = " << psize << "\n";
			size += psize;
		}
		else {
			//std::cout << "serialising a game object\n";
			m[size] = 'O';
			if (deleted) {
				m[size + 1] = 'D';
			}
			else {
				m[size + 1] = 'K';
			}
			size += 2;
			int gosize;
			gosize = (go.second)->serialize(&m[size]);
			//std::cout << "game object size = " << gosize << "\n";
			size += gosize;
		}
	}
	//std::cout << "total map size = " << size << "\n";
	std::vector<Address>::iterator it;
	for (it = addresses.begin(); it != addresses.end(); it++) {
		Message mess = Message(*it, m, size);
		(this->toSend).pushToEnd(mess);
	}
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
			else{
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
		}
	}
}

void Sender::stop() {
	std::cout << "Called stop on sender";
	(this->cont) = false;
}