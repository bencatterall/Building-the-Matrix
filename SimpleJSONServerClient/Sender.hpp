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
		Sender(Socket s);
		~Sender();
		void sendUpdateMessage(Address client, std::map<GameObjectGlobalID, GameObject> message);
		void sendAck(Address client,std::string event);
		void run();
		void stop();
};