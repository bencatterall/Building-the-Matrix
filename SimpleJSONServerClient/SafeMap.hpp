#include <map>
#include <mutex>
#include "CommonMinimal.hpp"
class GameObject;

class GameObject {
	public:
		int getID();
};

template<class K,class V>
class SafeMap {
	private:
		std::map<K,V> map;
		std::mutex lock;
	public:
		SafeMap();
		~SafeMap();
		V get(K key);
		void deleteEntry(K key);
		void put(K key, V value);
		std::map<K, V> getSnapshot();
};