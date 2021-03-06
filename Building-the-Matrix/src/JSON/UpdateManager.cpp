
#include "../Common.hpp"
#include "../GameObject.hpp"
#include "../ObjectManager.hpp"
#include "../LocationComponent.hpp"
#include "../ObjectManager.hpp"
#include "UpdateManager.hpp"

int UpdateManager::nextID = 1;
std::mutex UpdateManager::updManagerIndexMutex;

void UpdateManager::registerObject(GameObjectID gameObjectID, GameObjectGlobalID globalID) {
	ObjectManager& objManager = ObjectManager::getInstance();
	std::shared_ptr<GameObject> gameObject = objManager.getObject(gameObjectID);
	gameObject->setGlobalID(globalID);
}

StringBuffer UpdateManager::SerializeServer(GameObjectID gameObjectID) {
	ObjectManager& objManager = ObjectManager::getInstance();
	std::shared_ptr<GameObject> gameObject = objManager.getObject(gameObjectID);
	const char* json = "{\"ObjectID\":null,\"LocationComponent\":null}";	//Handling LocationComponent???
	Document d;
	d.Parse(json);

	Value& s1 = d["ObjectID"];
	s1.SetInt(gameObject->getGlobalID());

	Value& s2 = d["LocationComponent"];


	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	return buffer;
}

void /*(GameInput?)*/ UpdateManager::DeserializeServer(const char* jsonStr) {

	Document d;
	d.Parse(jsonStr);

	Value& s1 = d["ObjectID"];
	int objID = s1.GetInt();

	Value& s2 = d["InputUp"];
	int id = s2.GetInt();

	Value& s3 = d["InputDown"];
	 id = s3.GetInt();

	Value& s4 = d["InputLeft"];
	 id = s4.GetInt();

	Value& s5 = d["InputRight"];
	 id = s5.GetInt();

	//Construct GameInput object to pass to Physics?
}

/*
StringBuffer UpdateManager::SerializeClient(GameInput* gameInput) {
	const char* json = "{\"ObjectID\":null,\"InputMatrix\":null}";
	Document d;
	d.Parse(json);

	Value& s1 = d["ObjectID"];
	s1.SetInt(gameObject->getGlobalID());

	Value& s2 = d["LocationComponent"];

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	d.Accept(writer);

	return buffer;
}

void UpdateManager::DeserializeClient(const char* jsonStr) {

	Document d;
	d.Parse(jsonStr);

	Value& s1 = d["ObjectID"];
	int objID = s1.GetInt();

	Value& s2 = d["LocationComponent"];

}
*/