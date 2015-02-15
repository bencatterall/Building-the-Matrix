#include "../src/CommonMinimal.hpp"

#ifdef TEST_GRAPHICS

#include <memory>

#include "../src/Common.hpp"
#include "../src/ObjectManager.hpp"
#include "../src/GameObject.hpp"

///
/// See https://github.com/philsquared/Catch/blob/master/docs/tutorial.md 
/// for examples of writing tests
///
TEST_CASE ("TESTING", "[Test]") {
	ObjectManager& objManager = ObjectManager::getInstance();
	std::shared_ptr<GameObject> gameObject1 = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> gameObject2 = std::make_shared<GameObject>();

	objManager.addObject(gameObject1);
	objManager.addObject(gameObject2);

	SECTION("Testing adding") {
		REQUIRE(gameObject1 == objManager.getObject(gameObject1->getID()));
		REQUIRE(gameObject2 == objManager.getObject(gameObject2->getID()));
	}
	SECTION("Testing removal") {
		REQUIRE(objManager.removeObject(gameObject1->getID()) == true);
		REQUIRE(objManager.removeObject(gameObject2->getID()) == true);
	}
	SECTION("Testing adding after removal") {
		REQUIRE(gameObject1 == objManager.getObject(gameObject1->getID()));
		REQUIRE(gameObject2 == objManager.getObject(gameObject2->getID()));
		REQUIRE(objManager.removeObject(gameObject1->getID()) == true);
		REQUIRE(objManager.removeObject(gameObject2->getID()) == true);
	}
}


#endif