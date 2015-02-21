#include "../src/Common.hpp"

#ifdef TEST
#ifdef TEST_PHYSICS

#include <Catch/catch.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>
#include "../../Dependencies/glew/glew.h"
#include "../../Dependencies/GLFW/glfw3.h"
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

	AABB cubeAABB = *phy.getWorldAABB();

	REQUIRE(cubeAABB.getCenter() == vec3(1.0f, 2.0f, 3.0f));
	//REQUIRE(cubeAABB.getXSpan() == 8.0f);

	PhysicsMaths::stepObject(phy, 1.0f);

	REQUIRE(phy.getX() == vec3(1.0f, 3.0f, 3.5f));
	REQUIRE(phy.getV() == vec3(0.0f, 1.0f, 1.0f));

	Cube cube1 = Cube(vec3(0, 0, 0)), cube2 = Cube(vec3(0, 0, 21));
	cube2.getPhysicsComponent()->setV(vec3(0,0,-1));
	PhysicsObject physA = *cube1.getPhysicsComponent(), physB = *cube2.getPhysicsComponent();
	PhysicsMaths::stepObject(physA, 1.02f);
	PhysicsMaths::stepObject(physB, 1.02f);
	vec3 aCen = physA.getLocalAABB().getCenter();
	vec3 bCen = physB.getLocalAABB().getCenter();

	// Transform these to world space
	glm::mat4 matA = glm::translate(glm::mat4x4(1.0f), cube1.getLocationComponent()->getPosition());
	glm::mat4 matB = glm::translate(glm::mat4x4(1.0f), cube2.getLocationComponent()->getPosition());
	glm::vec4 aCen4 = glm::vec4(aCen.x, aCen.y, aCen.z, 1.0f);
	glm::vec4 bCen4 = glm::vec4(bCen.x, bCen.y, bCen.z, 1.0f);
	aCen = vec3(matA * aCen4);
	bCen = vec3(matB * bCen4);

	// Calculate relative velocity and position
	if (aCen == bCen) return;
	vec3 sDiff = aCen - bCen;
	vec3 sDiffNormal = glm::normalize(sDiff);
	vec3 vDiff = physA.getV() - physB.getV();

	// Calculate relative velocity along normal direction
	float velDelAlongCollisionNormal = glm::dot(vDiff, sDiffNormal);

	// Do not resolve if they are separating already
	if (velDelAlongCollisionNormal > 0.0f)
		return;

	// Choose minimal restitution
	float e = std::min(physA.getRest(), physB.getRest());

	// Calculate impulse vec3
	float j = -(1.0f + e) * velDelAlongCollisionNormal;
	j = j / (physA.getInvMass() + physB.getInvMass());
	vec3 impulse = j * sDiffNormal;

	// Apply impulse in an amount proportional to its mass proportion.
	float mass_sum = (physA.getMass() + physB.getMass());
	physA.setV(physA.getV() - impulse * physA.getMass() / mass_sum);
	physB.setV(physB.getV() + impulse * physB.getMass() / mass_sum);
	
	REQUIRE(physA.getV() == vec3(0, 0, -1));
	REQUIRE(physB.getV() == vec3(0, 0, 0));

	//Breakpoint purposes
	REQUIRE(true);
}
#endif
#endif