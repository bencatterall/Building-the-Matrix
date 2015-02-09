/**
	clientMain.cpp
	Purpose: Controls a client instance bounded to arg[1] address and arg[2] port number, and sending to
	server at arg[3] address and arg[4] port number

*/

#include "Address.hpp"
#include "Socket.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

bool cont;
Address server;
Socket mySocket;

//receives server updates from the server
void receive() {
	while (cont) {
		Address sender;
		unsigned char buffer[256];
		int bytes_read = mySocket.receive(sender, (char *)buffer, sizeof(buffer));

		if (bytes_read >= 0) {
			std::string recmessage((char *)buffer);
			std::cout << "Client received message: " << recmessage << "\n";
		}
	}
}

//used to send updates when user input occurs
void send() {

}

//keeps this client running
int main(int argc, char **argv) {
	//expect IP address to be the localhost of the machine: "127.0.0.1"
	if (argc != 5) {
		std::cerr << "Usage: create server by providing <local IP address> <local Port> <server IP address> <server Port>";
		std::exit(EXIT_FAILURE);
	}

	std::string IP = argv[1];
	std::string port = argv[2];
	std::string serverIP = argv[3];
	std::string serverPort = argv[4];
	unsigned short portNo = atoi(port.c_str());
	unsigned short serverPortNo = atoi(serverPort.c_str());
	int acc = 0;
	int IPValues[4];
	int serverIPValues[4];
	size_t pos = 0;
	while ((pos = IP.find('.')) != std::string::npos) {
		IPValues[acc] = atoi((IP.substr(0, pos)).c_str());
		IP = IP.substr(pos + 1, (IP.length() - (pos + 1)));
		acc++;
	}
	IPValues[3] = atoi(IP.c_str());
	acc++;
	if (acc != 4) {
		std::cerr << "Deformed client IP address given";
		std::exit(EXIT_FAILURE);
	}
	acc = 0;
	while ((pos = serverIP.find('.')) != std::string::npos) {
		serverIPValues[acc] = atoi((serverIP.substr(0, pos)).c_str());
		serverIP = serverIP.substr(pos + 1, (serverIP.length() - (pos + 1)));
		acc++;
	}
	serverIPValues[3] = atoi(serverIP.c_str());
	acc++;
	if (acc != 4) {
		std::cerr << "Deformed server IP address given";
		std::exit(EXIT_FAILURE);
	}

	//make address for server to send to from commandline arguments (alternatively we could make a
	//config file?)
	Address *serverAddr = new Address(serverIPValues[0], serverIPValues[1], serverIPValues[2], serverIPValues[3], serverPortNo);
	server = *serverAddr;

	//make address of this client
	Address *clientAddr = new Address(IPValues[0], IPValues[1], IPValues[2], IPValues[3], portNo);

	// create socket client will be on
	if (!mySocket.openSocket(clientAddr->getAddress(), clientAddr->getPort()))
	{
		std::cerr << "Failed to create client socket\n";
		std::exit(EXIT_FAILURE);
	}

	// send a packet to the server for a test
	const char data[] = "LOGIN";
	if (!(mySocket.sendSingle(*serverAddr, data, sizeof(data)))) {
		std::cerr << "Failed to send message";
		std::exit(EXIT_FAILURE);
	}
	bool confirmed = false;
	//receive login confirmation
	while (!confirmed) {
		//resend login request if server doesn't reply within a second
		std::this_thread::sleep_for(std::chrono::seconds(1));
		Address sender;
		unsigned char buffer[256];
		int bytes_read = mySocket.receive(sender, (char *)buffer, sizeof(buffer));
		if (bytes_read >= 0) {
			std::string recmessage((char *)buffer);
			std::cout << "Client received message: " << recmessage << "\n";
			confirmed = true;
		}
		else {
			const char data[] = "LOGIN";
			if (!(mySocket.sendSingle(*serverAddr, data, sizeof(data)))) {
				std::cerr << "Failed to send message";
				std::exit(EXIT_FAILURE);
			}
		}
	}
	
	cont = true;
	std::thread rec(receive);
	//for now, quit when enter is pressed
	if (std::cin.get() == '\n') {
		cont = false;
		const char data[] = "LOGOUT";
		if (!(mySocket.sendSingle(*serverAddr, data, sizeof(data)))) {
			std::cerr << "Failed to send message";
			std::exit(EXIT_FAILURE);
		}
	}	
	
	rec.join();
	mySocket.~Socket();
}