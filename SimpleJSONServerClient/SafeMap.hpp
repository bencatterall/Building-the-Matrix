#ifndef SAFEMAP_H
#define SAFEMAP_H

#include <map>
#include <mutex>
#include "CommonMinimal.hpp"
#include "GameObject.hpp"

template<class K,class V>
class SafeMap {
	private:
		std::map<K,V> map;
		std::mutex lock;
	public:
		SafeMap();
		~SafeMap();
		SafeMap(const SafeMap& map); //don't want compiler generating this function as mutexes are uncopyable
		V get(K key);
		void deleteEntry(K key);
		void put(K key, V value);
		std::map<K, V> getSnapshot();
};
#endif