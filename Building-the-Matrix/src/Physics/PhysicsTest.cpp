#include <Catch\catch.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>

#include "../CommonMinimal.hpp"
#include "../GameObject.hpp"
#include "../LocationComponent.hpp"
#include "../RenderableComponent.hpp"
#include "PhysicsObject.hpp"
#include "PhysicsMaths.hpp"

using glm::vec3;

TEST_CASE("Testing Physics", "[physics]"){
	
	SECTION("Linear Motion"){
		glm::mat4x4 i4 = glm::mat4x4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);

		std::shared_ptr<LocationComponent> loc = std::make_shared<LocationComponent>(glm::vec3(1, 2, 3), i4, 1.0);
		std::shared_ptr<RenderableComponent> ren = std::make_shared<RenderableComponent>();
		// TODO: Set up ren with a 2-unit cube centered on (1,2,3)
		PhysicsObject phy = PhysicsObject(loc, ren);
		phy.setA(vec3(0, 0, 1));
		phy.setV(vec3(0, 1, 0));
		PhysicsMaths::stepObject(phy, 1.0f);

		REQUIRE(phy.getV() == vec3(0, 1, 1));
		REQUIRE(phy.getX() == vec3(1, 3, 3.5));
	}

	SECTION("Collision check"){
		;
	}

	SECTION("Collision resolution"){
		;
	}
}