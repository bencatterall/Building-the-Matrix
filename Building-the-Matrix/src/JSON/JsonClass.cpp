#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

/*
RapidJSON

RapidJSON allows easy creatiion/modification of JSON strings using the Document Object Model
(DOM). JSON strings can be parsed into DOM, where you can query/modify fields, and then
convert back to JSON for sending. The Document type represents JSON strings as a DOM tree: an 
object is the root of the tree, its children are the field identifiers, and the children
of those are the field values. Each JSON value is stored in a type Value, which has the
functions required for getting or setting field values.

This class is an example of using RapidJSON for serialising classes: not necessary for the project and can be deleted whenever.

*/

using namespace rapidjson;

/*
A basic class which holds a couple of examples of GameObject fields.
*/
class JsonClass {

public:

	bool inWorld;
	int objectID;

	// Class constructor
	JsonClass(bool b, int i) {
		inWorld = b;
		objectID = i;
	}

	/*
	Serializes an instance of this class. A default JSON string is created, we parse it
	into a DOM, set the correct object values, and re-stringify by writing to a string
	buffer.
	*/
	StringBuffer Serialize() {
		const char* json = "{\"InWorld\":null,\"ObjectID\":null}";
		Document d;
		d.Parse(json);

		Value& s1 = d["InWorld"];
		s1.SetBool(inWorld);

		Value& s2 = d["ObjectID"];
		s2.SetInt(objectID);
	
		StringBuffer buffer;
		Writer<StringBuffer> writer(buffer);
		d.Accept(writer);

		const char* newJson = buffer.GetString();

		std::cout << "JSON string:" << std::endl;
		std::cout << buffer.GetString() << std::endl;

		return buffer;

	}
	
	/*
	Deserializes a JSON string by parsing to a DOM, and simply extracting the field values.
	*/
	void Deserialize(const char* jsonStr) {

		Document d;
		d.Parse(jsonStr);

		Value& s1 = d["InWorld"];
		bool world = s1.GetBool();

		Value& s2 = d["ObjectID"];
		int id = s2.GetInt();
		
		std::cout << "Extracted field values" << std::endl;
		std::cout << "In World: " << world << std::endl;
		std::cout << "Object ID: " << id << std::endl;
		
	}
};
/*
int main() {

	bool b = true;
	int i = 7;

	JsonClass js(b, i);

	StringBuffer jsonBuf = js.Serialize();

	std::cout << std::endl;

	const char* jsonString = jsonBuf.GetString();

	js.Deserialize(jsonString);

	int j;
	std::cin >> j;
	return 0;
}*/