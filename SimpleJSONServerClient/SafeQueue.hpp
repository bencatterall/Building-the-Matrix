#ifndef SAFEQUEUE_H
#define SAFEQUEUE_H

#include <queue>
#include <mutex>
#include "GameObject.hpp"
#include "CommonMinimal.hpp"
#include "Update.hpp"

template<class C>
class SafeQueue {
private:
	std::queue<C> queue;
	std::mutex lock;
public:
	SafeQueue();
	~SafeQueue();
	SafeQueue(const SafeQueue& map); //don't want compiler generating this function as mutexes are uncopyable
	C popFromFront();
	void pushToEnd(C update);
	bool isEmpty();
};
#endif