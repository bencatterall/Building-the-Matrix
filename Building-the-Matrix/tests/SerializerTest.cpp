#ifdef TEST
#ifdef TEST_UPDATES

#include <Catch/catch.hpp>

#include "../src/Common.hpp"
#include "../src/GameObject.hpp"
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

    SECTION("Consecutive deserialization") {
        int testInt = 105;
        int testInt2 = 106;
        length = serialize.packInt(data, testInt);
        REQUIRE(length == 4);
        length = serialize.packInt(data, testInt2);
        REQUIRE(length == 4);
        REQUIRE(testInt == serialize.unpackInt(data, p));
        REQUIRE(testInt2 == serialize.unpackInt(data, p));
    }

    // TODO: add tests for (un/)serialization of std::map<GameObjectGlobalID, GameObject>
}

#endif
#endif