#include "SafeQueue.hpp"
#include <iostream>

SafeQueue<Update>::SafeQueue() {
	(this->queue) = std::queue<Update>();
}

SafeQueue<Update>::~SafeQueue() {}

Update SafeQueue<Update>::popFromFront() {
	(this->lock).lock();
	if ((this->queue).empty()) {
		std::cout << "queue was empty\n";
		(this->lock).unlock();
		throw 1;
	}
	else {
		Update u = (this->queue).front();
		(this->queue).pop();
		std::cout << "popped element, size now " << (this->queue).size() << "\n";
		(this->lock).unlock();
		return u;
	}
}

void SafeQueue<Update>::pushToEnd(Update update) {
	(this->lock).lock();
	(this->queue).push(update);
	std::cout << "pushed element, size now " << (this->queue).size() << "\n";
	(this->lock).unlock();
}