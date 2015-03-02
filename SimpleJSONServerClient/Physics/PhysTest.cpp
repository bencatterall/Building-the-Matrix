#include "..\Common.hpp"

#ifdef TEST
#ifdef TEST_PHYSICS
#include "../../Building-The-Matrix/Dependencies/Catch/catch.hpp"
#include "../Player.hpp"
#include "PhysicsMaths.hpp"

TEST_CASE("", "[a]"){
	Player play = Player(1982);
	GameObject game = (GameObject) play;
	std::shared_ptr<PhysicsObject> phys = game.physComp;
	REQUIRE(phys->getA() == vec3());
	PhysicsMaths::acceleratePlayer(phys);
	REQUIRE(phys->getA() == vec3(0, 0, -1));
	PhysicsMaths::reversePlayer(phys);
	REQUIRE(phys->getA() == vec3(0, 0, 0));

	REQUIRE(true);
}
#endif
#endif
