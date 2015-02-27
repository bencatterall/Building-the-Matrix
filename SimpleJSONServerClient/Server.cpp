//#define TEST

#include "Socket.hpp"
#include "Address.hpp"
#include "ClientState.hpp"
#include "UpdateManager.hpp"
#include "Sender.hpp"
#include "Player.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <vector>
#include <map>
#include <chrono>
#include "Physics/Simulator.hpp"

/**
ServerMain.cpp
Purpose: Controls a server instance bounded to arg[1] address and arg[2] port number

*/

UpdateManager& updateManager = UpdateManager::getInstance();
Socket mySocket;
std::vector <Address> clients;
std::vector <std::pair<Address, GameObjectGlobalID>> playerIDs;
std::map <Address, ClientState> clientStates;
Sender sender;
bool contMain;

bool prefixMatch(std::string message, std::string prefix) {
	return message.compare(0, prefix.length(), prefix) == 0;
}

void broadcast() {
	while (contMain) {
		//send snapshot back
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> toSend = updateManager.flushUpdates();
		if (!toSend.empty()) {
			//std::cout << "Sending game snapshot \n";
			sender.sendUpdateMessage(clients, toSend);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void quit() {
	if (std::cin.get() == '\n') {
		contMain = false;
	}
}

void physics() {
	auto timer = std::chrono::system_clock::now();
	while (contMain) {
		//TODO: RUN PHYSICS HERE
		Simulator & physicsSimulator = Simulator::getInstance();
		auto nextTime = std::chrono::system_clock::now();
		std::chrono::duration<float> timestepDur = nextTime - timer;
		timer = nextTime;
		physicsSimulator.tick(timestepDur.count());
		while (physicsSimulator.tickCount > 100){
			// std::cout << "2 Physics seconds has passed\n";
			physicsSimulator.tickCount -= 100;
		}
		// Try to avoid being too fast
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}

void respond_logout_client(Address toLogout) {
	for (auto it = clients.begin(); it < clients.end(); it++) {
		if ((it->getAddress() == toLogout.getAddress()) && (it->getPort() == toLogout.getPort())) {
			clients.erase(it);
			//std::cout << "removed from current connected clients \n";
			break;
		}
		else {
			//const char data[] = "A FELLOW PLAYER HAS LEFT THE GAME";
			//sender.sendAck((*it), data);
		}
	}
	for (auto it2 = playerIDs.begin(); it2 < playerIDs.end(); it2++) {
		if (((it2->first).getAddress() == toLogout.getAddress()) && ((it2->first).getPort() == toLogout.getPort())) {
			updateManager.remove(it2->second);
			playerIDs.erase(it2);
			//std::cout << "removed from current IDs \n";
			break;
		}
	}
}

void check_client_timeouts() {
	while (contMain) {
		for(auto it = clientStates.begin(); it != clientStates.end(); ) {
			if (it->second.timedOut()) {
				// logout client with address it->first
				std::cerr << "Client " << it->first.getHBOAddress() << " timed out - logging it out" << std::endl;
				respond_logout_client(it->first);
				it = clientStates.erase(it);
			} else {
				++it;
			}
		}
		// loop through all clients and check if timed out
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}
#ifndef TEST
int main(int argc, char **argv) {
	//setup server to run on port
	//expect IP address to be the localhost of the machine: "127.0.0.1"
	if (argc != 3) {
		std::cerr << "Usage: create server by providing <IP address> <Port>\n";
		std::exit(EXIT_FAILURE);
	}

	std::string IP = argv[1];
	std::string port = argv[2];
	unsigned short portNo = atoi(port.c_str());
	int acc = 0;
	int IPValues[4];
	size_t pos = 0;
	float playerZpos = 0.0f;
	while ((pos = IP.find('.')) != std::string::npos) {
		IPValues[acc] = atoi((IP.substr(0, pos)).c_str());
		IP = IP.substr(pos + 1, (IP.length() - (pos + 1)));
		acc++;
	}
	IPValues[3] = atoi(IP.c_str());
	acc++;
	if (acc != 4) {
		std::cerr << "Deformed IP address given\n";
		std::exit(EXIT_FAILURE);
	}

	//make address for server to run on from commandline arguments (alternatively we could make a
	//config file?)
	Address *serverAddr = new Address(IPValues[0], IPValues[1], IPValues[2], IPValues[3], portNo);

	// create socket server will be on
	if (!mySocket.openSocket(serverAddr->getAddress(), serverAddr->getPort()))
	{
		std::cerr << "Failed to create server socket\n";
		std::exit(EXIT_FAILURE);
	}

	std::cout << "connected to host " << serverAddr->getHBOAddress() << " on port " << serverAddr->getHBOPort() << "\n";

	std::vector < std::shared_ptr<GameObject> > initialObjects;
	//TODO: push initial objects

	
	updateManager.setInitialObjects(initialObjects);
	sender.setSocket(&mySocket);
	
	contMain = true;

	//start update manager run method in new thread
	std::thread makeUpdates(&UpdateManager::run, &updateManager);

	//start send loop in new thread
	std::thread sendMessages(&Sender::run, &sender);

	//start send update loop in new thread
	std::thread update(broadcast);

	std::thread t_physics(physics);

	//start listener fo quitting
	std::thread l_quit(quit);

	// thread to monitor clients and terminate if timed out
	std::thread t_timeout(check_client_timeouts);
	
	//input loops in this thread
	while (contMain) {
		//try to receive updates
		Address recFrom;
		unsigned char buffer[256];
		int bytes_read = mySocket.receive(recFrom, (char *)buffer, 256);
		
		if (bytes_read >= 0) {
			//check what type of message was received
			std::string message((char *)buffer);
			//std::cout << "received: " << message << "\n";
			//HANDLE LOGINS
			if (prefixMatch(message, "LOGIN")) {
				GameObjectGlobalID id;
				bool copy = false;
				std::cout << "Server received login request \n";
				for (Address s : clients) {
					if ((s.getAddress() == recFrom.getAddress()) && (s.getPort() == recFrom.getPort())) {
						//already received request but ACK not received by sender so just try again
						std::cout << "resending login ACK \n";
						copy = true;
						for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
							if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getAddress()) {
								id = e.second;
							}
						}
						break;
					}
				}
				if (!copy) {
					std::cout << "Added client to current list connected with address = " << recFrom.getHBOAddress() << " at port = " << recFrom.getHBOPort() << "\n";
					clients.push_back(recFrom);
					clientStates[recFrom] = ClientState(recFrom);
					//create their car for the game, generate a global ID too
					id = updateManager.getNextObjectID();
					playerIDs.push_back(std::pair<Address, GameObjectGlobalID>(recFrom, id));
					Player player = Player(id);
					std::shared_ptr<GameObject> p = std::make_shared<Player>(player);
					p->locComp->setPosition(glm::vec3(0.0f,50.0f,playerZpos));
					playerZpos += 25.0f;
					updateManager.queueUpdate(p);
				}
				char data[] = "LOGIN ACCEPTED     ";
				char *idparts = (char*)&id;
				//TODO: serialize using serializer
				data[15] = idparts[3];
				data[16] = idparts[2];
				data[17] = idparts[1];
				data[18] = idparts[0];
				sender.sendMessage(recFrom, (unsigned char *)data, 20);
			}
			//HANDLE LOGOUTS
			else if (prefixMatch(message, "LOGOUT")) {
				std::cout << "Server received logout request \n";
				//respond_logout_client(recFrom);
				for (auto it = clients.begin(); it < clients.end(); it++) {
					if ((it->getAddress() == recFrom.getAddress()) && (it->getPort() == recFrom.getPort())) {
						clients.erase(it);
						std::cout << "removed from current connected clients \n";
						break;
					}
					else {
						//const char data[] = "A FELLOW PLAYER HAS LEFT THE GAME";
						//sender.sendAck((*it), data);
					}
				}
				for (auto it2 = playerIDs.begin(); it2 < playerIDs.end(); it2++) {
					if (((it2->first).getAddress() == recFrom.getAddress()) && ((it2->first).getPort() == recFrom.getPort())) {
						//edit remove to prevent bug
						updateManager.remove(it2->second);
						playerIDs.erase(it2);
						std::cout << "removed from current IDs \n";
						break;
					}
				}
				//clientStates.erase(recFrom);
			}
			//HANDLE USER INPUT (SENT IN FORMAT <ACTION> <INT REPRESENTING KEY>)
			else if (prefixMatch(message, "PRESSED")) {
				// char key = buffer[8];
				int key = buffer[8];
				// int p = 0;
				// int key = Serializer::unpackInt(&buffer[8], p);
				std::cout << "User pressed " << (char)key << " (" << key << ")\n";
				for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
					if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getPort()) {
						std::shared_ptr<GameObject> p = updateManager.getGameObject(e.second);
						std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(p);
						player->keyPressed(key);
						//updateManager.queueUpdate(player);
						break;
					}
				}
				clientStates[recFrom].bump();
			}
			else if (prefixMatch(message, "UNPRESSED")) {
				// char key = buffer[10];
				int key = buffer[10];
				// int p = 0;
				// int key = Serializer::unpackInt(&buffer[10], p);
				std::cout << "User unpressed " << (char)key << " (" << key << ")\n";
				for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
					if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getPort()) {
						std::shared_ptr<GameObject> p = updateManager.getGameObject(e.second);
						std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(p);
						player->keyUnpressed(key);
						//updateManager.queueUpdate(player);
						break;
					}
				}
				clientStates[recFrom].bump();
			}
			//HANDLE HEAD ORIENTATION
			else if (prefixMatch(message, "PRY")) {
				//std::cout << "recieved pitch, roll and yaw from " << recFrom.getHBOAddress() << " " << recFrom.getHBOPort() << "\n";
				float *pry = new float[3];
				pry[0] = (float)((buffer[3] << 24) | (buffer[4] << 16) | (buffer[5] << 8) | buffer[6]);
				pry[1] = (float)((buffer[7] << 24) | (buffer[8] << 16) | (buffer[9] << 8) | buffer[10]);
				pry[2] = (float)((buffer[11] << 24) | (buffer[12] << 16) | (buffer[13] << 8) | buffer[14]);
				//std::cout << "p=" << pry[0] << " r=" << pry[1] << " y=" << pry[2] << "\n";
				for (std::pair<Address, GameObjectGlobalID> e : playerIDs) {
					if (e.first.getAddress() == recFrom.getAddress() && e.first.getPort() == recFrom.getPort()) {
						std::shared_ptr<GameObject> p = updateManager.getGameObject(e.second);
						std::shared_ptr<Player> player = std::dynamic_pointer_cast<Player>(p);
						player->setPRY(pry[0], pry[1], pry[2]);
						//updateManager.queueUpdate(player);
						break;
					}
				}
				clientStates[recFrom].bump();
			}
			else {
				std::cout << "Unknown message received\n";
				clientStates[recFrom].bump();
			}
		}
	}

	std::cout << "quitting the server\n";

	update.join();
	std::cout << "stopped updater\n";

	l_quit.join();
	std::cout << "stopped quit listener\n";
	
	t_physics.join();
	std::cout << "stopped receiver\n";

	t_timeout.join();
	std::cout << "stopped timeout checker\n";

	updateManager.stop();
	std::cout << "set update manager continue to false\n";
	makeUpdates.join();
	std::cout << "stopped update manager";
	
	sender.stop();
	std::cout << "set sender continue to false\n";
	sendMessages.join();
	std::cout << "stopped sender";

	std::cout << "server killed";
}
#endif
