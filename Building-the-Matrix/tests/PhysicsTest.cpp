#include "../src/Common.hpp"

#ifdef TEST
#ifdef TEST_PHYSICS

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

using glm::vec3;

TEST_CASE("Testing Physics", "[physics]"){
	int argc = 0;

	glutInit(&argc, nullptr);
	Game game;
	game.init();
	Cube cube = Cube(vec3(1.0f, 2.0f, 3.0f));
	REQUIRE(cube.getLocationComponent()->getPosition() == vec3(1.0f, 2.0f, 3.0f));
	PhysicsObject phy = *cube.getPhysicsComponent();
	phy.setA(vec3(0.0f, 0.0f, 1.0f));
	REQUIRE(phy.getA() == vec3(0.0f, 0.0f, 1.0f));
	phy.setV(vec3(0.0f, 1.0f, 0.0f));
	REQUIRE(phy.getV() == vec3(0.0f, 1.0f, 0.0f));
	phy.setLinDrag(0.0f);
	REQUIRE(phy.getLinDrag() == 0.0f);
	phy.setQuadDrag(0.0f);
	REQUIRE(phy.getQuadDrag() == 0.0f);

	SECTION("AABBs"){
		AABB cubeAABB = *phy.getWorldAABB();

		CHECK(cubeAABB.getCen() == vec3(1.0f, 2.0f, 3.0f));
		REQUIRE(cubeAABB.getXSpan() == 8.0f);
	}

	SECTION("Linear Motion"){
		PhysicsMaths::stepObject(phy, 1.0f);

		REQUIRE(phy.getV() == vec3(0.0f, 1.0f, 1.0f));
		REQUIRE(phy.getX() == vec3(1.0f, 3.0f, 3.5f));
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