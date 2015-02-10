#include "AABB.hpp"
#include "../Common.hpp"
#include "../ObjectManager.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"


namespace PhysicsMaths{
	// Simple first order approximation to motion given U, A, T.
	float PhysicsMaths::UATtoS(float U, float A, float T){
		return (U + (A * T) * 0.5f) * T;
	}

	// Simple first order approximation to motion given U, A, T.
	float PhysicsMaths::UATtoV(float U, float A, float T){
		return U + (A * T);
	}

	// Simple first order approximation to motion given U, A, T.
	vec3 PhysicsMaths::UATtoS(const vec3 U, const vec3 A, float T){
		vec3 vec = vec3();
		vec = (U + (A * T) * 0.5f) * T;
		return vec;
	}

	// Simple first order approximation to motion given U, A, T.
	vec3 PhysicsMaths::UATtoV(const vec3 U, const vec3 A, float T){
		return U + (A * T);
	}

	// Tests for global-axis aligned bounding box collision.
	// @param a,b - the two PhysicsObject items to check with
	// @return true if they collide
	bool simpleCollision(const PhysicsObject a, const PhysicsObject b){
		return simpleCollision(*a.getWorldAABB(), *b.getWorldAABB());
	}

	// Tests for global-axis aligned bounding box collision.
	// @param a,b - the two AABB items to check with
	// @return true if they collide
	bool simpleCollision(const AABB &a, const AABB &b){
		vec3 vec3Difference = a.getCen() - b.getCen();
		
		// If there exists a gap in the direction of a principal axis, then
		// there exists a plane normal to that axis which separates them.
		float xGap = vec3Difference.x - (a.getXSpan() + b.getXSpan()) * 0.5f;
		if (xGap > 0){
			return false;
		}
		float yGap = vec3Difference.y - (a.getYSpan() + b.getYSpan()) * 0.5f;
		if (yGap > 0){
			return false;
		}
		float zGap = vec3Difference.z - (a.getZSpan() + b.getZSpan()) * 0.5f;
		if (zGap > 0){
			return false;
		}
		return true;
	}

	void handleCollision(GameObjectID aID, GameObjectID bID){
		ObjectManager objMan = ObjectManager::getInstance();
		// TODO: Requires handling of complex collision

		PhysicsObject a = *objMan.getObject(aID)->getPhysicsComponent();
		PhysicsObject b = *objMan.getObject(bID)->getPhysicsComponent();
		vec3 aCen = a.getLocalAABB().getCen();
		vec3 bCen = b.getLocalAABB().getCen();
		// TODO: Transform these to world space

		// Calculate relative velocity and position
		vec3 sDiff = aCen - bCen;
		vec3 sDiffNormal = glm::normalize(sDiff);
		vec3 vDiff = a.getV() - b.getV();

		// Calculate relative velocity along normal direction
		float velDelAlongCollisionNormal = glm::dot(vDiff, sDiffNormal);

		// Do not resolve if they are separating already
		if (velDelAlongCollisionNormal > 0)
			return;

		// Choose minimal restitution
		float e = std::min(a.getRest(), b.getRest());

		// Calculate impulse vec3
		float j = -(1 + e) * velDelAlongCollisionNormal;
		j = j / (a.getInvMass() + b.getInvMass());
		vec3 impulse = j * sDiffNormal;

		// Apply impulse in an amount proportional to its mass proportion.
		float mass_sum = (a.getMass() + b.getMass());
		a.setV(a.getV() - impulse * a.getMass() / mass_sum);
		b.setV(b.getV() + impulse * b.getMass() / mass_sum);
	}

	void PhysicsMaths::stepObject(PhysicsObject physObj, float timestep){
		physObj.setX(UATtoS(physObj.getV(), physObj.getA(), timestep));
		physObj.setV(UATtoV(physObj.getV(), physObj.getA(), timestep));
	}
}
