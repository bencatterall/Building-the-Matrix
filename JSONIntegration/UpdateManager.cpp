
#include "GameObject.hpp"
#include "ObjectManager.hpp"
#include "LocationComponent.hpp"

#include "UpdateManager.hpp"

void UpdateManager::addObject(GameObject gameObject) {
	auto ret = gameObjects.insert(std::pair< int, GameObject >(gameObject->getGlobalID(), gameObject));
}

GameObject UpdateManager::findObject(int id) {
}

void UpdateManager::removeObject(int id) {

}

StringBuffer UpdateManager::Serialize(GameObject gameObject) {
	const char* json = "{\"ObjectID\":null,\"LocationComponent\":null}";	//Handling LocationComponent???
	Document d;
	d.Parse(json);

	Value& s1 = d["ObjectID"];
	s1.SetInt(gameObject.globalID);

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	return buffer;
}

void /*(GameInput?)*/ UpdateManager::Deserialize(const char* jsonStr) {

	Document d;
	d.Parse(jsonStr);

	Value& s1 = d["ObjectID"];
	int objID = s1.GetInt();

	Value& s2 = d["InputUp"];
	int id = s2.GetInt();

	Value& s3 = d["InputDown"];
	int id = s3.GetInt();

	Value& s4 = d["InputLeft"];
	int id = s4.GetInt();

	Value& s5 = d["InputRight"];
	int id = s5.GetInt();

	//Construct GameInput object to pass to Physics?

}