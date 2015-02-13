#include <queue>
#include <mutex>

class Update {
	public:
		GameObjectGlobalID getObjectID();
		GameObject getEditedObject();
};

template<class C>
class SafeQueue {
private:
	std::queue<C> queue;
	std::mutex lock;
public:
	SafeQueue();
	~SafeQueue();
	C popFromFront();
	void pushToEnd(C update);
};