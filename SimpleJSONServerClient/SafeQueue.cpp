#include "SafeQueue.hpp"
#include "Message.hpp"
#include <iostream>

SafeQueue<Update>::SafeQueue() {
	(this->queue) = std::queue<Update>();
}

SafeQueue<Update>::~SafeQueue() {}

bool SafeQueue<Update>::isEmpty() {
	(this->lock).lock();
	bool r = (this->queue).empty();
	(this->lock).unlock();
	return r;
}

Update SafeQueue<Update>::popFromFront() {
	(this->lock).lock();
	if ((this->queue).empty()) {
		//std::cout << "queue was empty\n";
		(this->lock).unlock();
		//throw 1;
	}
	else {
		Update u = (this->queue).front();
		(this->queue).pop();
		std::cout << "popped element from pending updates queue, size now " << (this->queue).size() << "\n";
		(this->lock).unlock();
		return u;
	}
}

void SafeQueue<Update>::pushToEnd(Update update) {
	(this->lock).lock();
	(this->queue).push(update);
	std::cout << "pushed element to pending updates queue, size now " << (this->queue).size() << "\n";
	(this->lock).unlock();
}

SafeQueue<Message>::SafeQueue() {
	(this->queue) = std::queue<Message>();
}

SafeQueue<Message>::~SafeQueue() {}

bool SafeQueue<Message>::isEmpty() {
	(this->lock).lock();
	bool r = (this->queue).empty();
	(this->lock).unlock();
	return r;
}

Message SafeQueue<Message>::popFromFront() {
	(this->lock).lock();
	if ((this->queue).empty()) {
		//std::cout << "queue was empty\n";
		(this->lock).unlock();
		//throw 1;
	}
	else {
		Message u = (this->queue).front();
		(this->queue).pop();
		std::cout << "popped element from messages to send queue, size now " << (this->queue).size() << "\n";
		(this->lock).unlock();
		return u;
	}
}

void SafeQueue<Message>::pushToEnd(Message message) {
	(this->lock).lock();
	(this->queue).push(message);
	std::cout << "pushed element to messages to send queue, size now " << (this->queue).size() << "\n";
	(this->lock).unlock();
}