#ifndef SAFEMAP_H
#define SAFEMAP_H

#include <map>
#include <mutex>
#include "CommonMinimal.hpp"
#include "GameObject.hpp"
#include <memory>

template<class K,class V>
class SafeMap {
	private:
		std::map<K,std::shared_ptr<V>> map;
		std::mutex lock;
	public:
		SafeMap();
		~SafeMap();
		SafeMap(const SafeMap& map); //don't want compiler generating this function as mutexes are uncopyable
		std::shared_ptr<V> get(K key);
		void deleteEntry(K key);
		void put(K key, std::shared_ptr<V> value);
		std::map<K, std::shared_ptr<V>> getSnapshot(bool flush);
		int count(K key);
};
#endif