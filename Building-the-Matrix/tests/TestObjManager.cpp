#include "../src/Common.hpp"
#ifdef TEST

#include <memory>

#include "../src/ObjectManager.hpp"
#include "../src/GameObject.hpp"
#define CATCH_CONFIG_MAIN
#include "../Dependencies/Catch/catch.hpp"
///
/// See https://github.com/philsquared/Catch/blob/master/docs/tutorial.md 
/// for examples of writing tests
///

TEST_CASE ("TESTING", "[Test]") {
	std::shared_ptr<ObjectManager> objManager = std::make_shared<ObjectManager>();
	std::shared_ptr<GameObject> gameObject1 = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> gameObject2 = std::make_shared<GameObject>();

	objManager->addObject(gameObject1);
	objManager->addObject(gameObject2);

	SECTION("Testing adding") {
		REQUIRE(gameObject1 == objManager->getObject(gameObject1->getID()));
		REQUIRE(gameObject2 == objManager->getObject(gameObject2->getID()));
	}
	SECTION("Testing removal") {
		REQUIRE(objManager->removeObject(gameObject1->getID()) == true);
		REQUIRE(objManager->removeObject(gameObject2->getID()) == true);

	}
	SECTION("Testing adding after removal") {
		REQUIRE(gameObject1 == objManager->getObject(gameObject1->getID()));
		REQUIRE(gameObject2 == objManager->getObject(gameObject2->getID()));
	}
}


#endif