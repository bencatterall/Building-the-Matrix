#include "../src/CommonMinimal.hpp"

#ifdef TEST
#ifdef TEST_PHYSICS

#include <Catch/catch.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "../src/Cube.hpp"
#include "../src/GameObject.hpp"
#include "../src/LocationComponent.hpp"
#include "../src/RenderableComponent.hpp"
#include "../src/Physics/AABB.hpp"
#include "../src/Physics/PhysicsObject.hpp"
#include "../src/Physics/PhysicsMaths.hpp"

using glm::vec3;


TEST_CASE("Testing Physics", "[physics]"){
	Cube cube = Cube();
	cube.getLocationComponent()->setPosition(vec3(1, 2, 3));
	PhysicsObject phy = *cube.getPhysicsComponent();
	phy.setA(vec3(0, 0, 1));
	phy.setV(vec3(0, 1, 0));

	SECTION("AABBs"){
		AABB cubeAABB = *phy.getWorldAABB();

		REQUIRE(cubeAABB.getCen() == vec3(1, 2, 3));
		CHECK(cubeAABB.getXSpan() == 2.0f);
	}

	SECTION("Linear Motion"){
		PhysicsMaths::stepObject(phy, 1.0f);

		REQUIRE(phy.getV() == vec3(0, 1, 1));
		REQUIRE(phy.getX() == vec3(1, 3, 3.5f));
	}

	SECTION("Collision check"){
		;
	}

	SECTION("Collision resolution"){
		;
	}
}
#endif
#endif