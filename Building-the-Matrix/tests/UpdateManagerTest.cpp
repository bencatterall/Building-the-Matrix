/*#include "../src/Common.hpp"

#ifdef TEST
#ifdef TEST_UPDATES

#include <Catch/catch.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include <freeglut/glut.h>

#include "../src/Game.hpp"
#include "../src/Cube.hpp"
#include "../src/GameObject.hpp"
#include "../src/LocationComponent.hpp"
#include "../src/RenderableComponent.hpp"
#include "../src/Physics/AABB.hpp"
#include "../src/Physics/PhysicsObject.hpp"
#include "../src/Physics/PhysicsMaths.hpp"

#include "../src/Serializer.hpp"
TEST_CASE("Testing Update Manager", "[update]"){
	Serializer serialize;
	char data[100];
	int length = 0;
	int p = 0;
	SECTION("Testing floats") {
		float testFloat = 10.0f;
		length = serialize.packFloat(data, testFloat);
		REQUIRE(length == 4);
		float result = serialize.unpackFloat(data, p);
		REQUIRE(testFloat == result);
	}

	SECTION("Testing ints") {
		int testInt = 105;
		length = serialize.packInt(data, testInt);
		REQUIRE(length == 4);
		REQUIRE(testInt == serialize.unpackInt(data, p));
	}

	SECTION("Testing bytes") {
		char testByte = 10;
		length = serialize.packByte(data, testByte);
		REQUIRE(length == 1);
		REQUIRE(testByte == serialize.unpackByte(data, p));
	}


	SECTION("Testing strings") {
		std::string testStr = "HELLO world! ";
		length = serialize.packString(data, testStr);
		REQUIRE(length == testStr.length() + 1); //null bytes
		REQUIRE(testStr == serialize.unpackString(data, p));
	}
}
#endif
#endif */