#include "../Common.hpp"

#ifdef TEST
#ifdef TEST_PHYSICS
#include "../../Building-The-Matrix/Dependencies/Catch/catch.hpp"
#include "../Player.hpp"
#include "PhysicsMaths.hpp"

TEST_CASE("", "[a]"){

	vec3 nzVec = vec3(0, 0, -1);

	Player player1 = Player(1982);
	GameObject game1 = (GameObject) player1;
	std::shared_ptr<PhysicsObject> phys1 = game1.physComp;
	REQUIRE(phys1->getA() == vec3());
	phys1->acceleratePlayer();
	REQUIRE(phys1->getA() == nzVec);
	phys1->reversePlayer();
	REQUIRE(phys1->getA() == vec3(0, 0, 0));
	phys1->setX(vec3(0, 0, -21));
	REQUIRE(phys1->getX() == vec3(0, 0, -21));

	Player player2 = Player(1999);
	GameObject game2 = (GameObject)player2;
	std::shared_ptr<PhysicsObject> phys2 = game2.physComp;
	phys2->setX(vec3(0, 0, 0));
	phys2->acceleratePlayer();
	REQUIRE(phys2->getA() == nzVec);
	REQUIRE(phys2->getX() == vec3());
	
	phys1->stepObject(2.0f);
	REQUIRE(phys1->getA() == vec3());
	REQUIRE(phys1->getV() == vec3());
	REQUIRE(phys1->getX() == vec3(0, 0, -21));

	phys2->stepObject(2.0f);
	REQUIRE(phys2->getA() == nzVec);
	REQUIRE(glm::dot(phys2->getV(), nzVec) > 0);
	REQUIRE(glm::dot(phys2->getX(), nzVec) > 0);

	// All tests pass up to here. Set-up is fine

	REQUIRE(PhysicsMaths::simpleCollision(*phys1->getWorldAABB(), *phys2->getWorldAABB()));

	REQUIRE(true);
}
#endif
#endif
