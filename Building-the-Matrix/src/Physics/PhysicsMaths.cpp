#include <algorithm>

#include "../GameObject.hpp"
#include "../ObjectManager.hpp"
#include "../RenderableComponent.hpp"
#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"


namespace PhysicsMaths{
	// Simple first order approximation to motion given U, A, T.
	float UATtoS(float U, float A, float T){
		return (U + (A * T) * 0.5f) * T;
	}

	// Simple first order approximation to motion given U, A, T.
	float UATtoV(float U, float A, float T){
		return U + (A * T);
	}

	// Simple first order approximation to motion given U, A, T.
	vec3 PhysicsMaths::UATtoS(const vec3 U, const vec3 A, float T){
		vec3 vec = vec3();
		vec = (U + (A * T) * 0.5f) * T;
		return vec;
	}

	// Simple first order approximation to motion given U, A, T.
	vec3 UATtoV(const vec3 U, const vec3 A, float T){
		return U + (A * T);
	}

	// Tests for global-axis aligned bounding box collision.
	// @param a,b - the two PhysicsObject items to check with
	// @return true if they collide
	bool simpleCollision(const GameObjectID a, const GameObjectID b){
		ObjectManager & obj = ObjectManager::getInstance();
		GameObject aObj = *obj.getObject(a);
		GameObject bObj = *obj.getObject(b);
		return simpleCollision(*aObj.getPhysicsComponent(), *bObj.getPhysicsComponent());
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
		ObjectManager& objMan = ObjectManager::getInstance();
		GameObject objA = *objMan.getObject(aID);
		GameObject objB = *objMan.getObject(bID);
		PhysicsObject physA = *objA.getPhysicsComponent();
		PhysicsObject physB = *objB.getPhysicsComponent();
		vec3 aCen = physA.getLocalAABB().getCen();
		vec3 bCen = physB.getLocalAABB().getCen();
		
		// Transform these to world space
		glm::mat4 matA = objA.getRenderableComponent()->getProjectionMatrix();
		glm::mat4 matB = objB.getRenderableComponent()->getProjectionMatrix();
		aCen = vec3(matA * glm::vec4(aCen.x, aCen.y, aCen.z, 0));
		bCen = vec3(matB * glm::vec4(bCen.x, bCen.y, bCen.z, 0));

		// Calculate relative velocity and position
		vec3 sDiff = aCen - bCen;
		vec3 sDiffNormal = glm::normalize(sDiff);
		vec3 vDiff = physA.getV() - physB.getV();

		// Calculate relative velocity along normal direction
		float velDelAlongCollisionNormal = glm::dot(vDiff, sDiffNormal);

		// Do not resolve if they are separating already
		if (velDelAlongCollisionNormal > 0)
			return;

		// Choose minimal restitution
		float e = std::min(physA.getRest(), physB.getRest());

		// Calculate impulse vec3
		float j = -(1 + e) * velDelAlongCollisionNormal;
		j = j / (physA.getInvMass() + physB.getInvMass());
		vec3 impulse = j * sDiffNormal;

		// Apply impulse in an amount proportional to its mass proportion.
		float mass_sum = (physA.getMass() + physB.getMass());
		physA.setV(physA.getV() - impulse * physA.getMass() / mass_sum);
		physB.setV(physB.getV() + impulse * physB.getMass() / mass_sum);
	}

	void stepObject(PhysicsObject physObj, float timestep){
		physObj.setX(UATtoS(physObj.getV(), physObj.getA(), timestep));
		vec3 newV = UATtoV(physObj.getV(), physObj.getA(), timestep);
		vec3 acc = physObj.getA();
		// Drag calculations
		float newVLength = glm::length(newV);
		// If we are speeding up
		if (glm::dot(acc, newV) > 0){
			acc = acc - (physObj.getQuadDrag()*newVLength + physObj.getLinDrag()) * 1.0f * newV;
			if (glm::dot(acc, newV) < 0){
				physObj.setA(vec3());
			}
			else{
				physObj.setA(acc);
			}
		}
		else{
			// If we are slowing down
			if (glm::dot(acc, newV) < 0){
				acc = acc + (physObj.getQuadDrag()*newVLength + physObj.getLinDrag()) * 1.0f * newV;
				physObj.setA(vec3());
			}
		}
		physObj.setV(newV);
	}



	std::vector<glm::vec3> convertGLfloatToVec3(std::vector<GLfloat> data) {
		std::vector<glm::vec3> newData;

		for (size_t i = 0; i < data.size(); i+=3) {
			newData.push_back(glm::vec3(data.at(i), data.at(i + 1), data.at(i + 2)));
		}

		return newData;
	}

	const glm::vec3 translateVertex(const glm::mat4x4 matrix, const vec3 vector){
		return vec3(matrix * glm::vec4(vector.x, vector.y, vector.z, 0));
	}

	const std::shared_ptr<vertexVector> translateVertexVector(const glm::mat4x4 matrix, const std::shared_ptr<vertexVector> vertices) {
		std::shared_ptr<vertexVector> result = std::make_shared<vertexVector>(*vertices);
		for (size_t i = 0; i < result->size(); i++){
			result->assign(i, translateVertex(matrix, result->at(i)));
		}
		return result;
	}

	void acceleratePlayer(const GameObjectID id){
		GameObject obj = *ObjectManager::getInstance().getObject(id);
		PhysicsObject phys = *obj.getPhysicsComponent();
		vec3 dir = phys.getOrientation();
		float speed = glm::length(dir);
		vec3 A = phys.getA();
		// TODO: Consider further mechanisms for determining power
		// TODO: Adjust arbitrary constant according to playtesting
		phys.setA(A + glm::normalize(dir)*(5-speed));
	}

	bool complexCollision(const GameObjectID a, const GameObjectID b){
		ObjectManager & obj = ObjectManager::getInstance();
		std::shared_ptr<PhysicsObject> aObj = obj.getObject(a)->getPhysicsComponent();
		std::shared_ptr<PhysicsObject> bObj = obj.getObject(b)->getPhysicsComponent();
		std::shared_ptr<vertexVector> aBox = aObj->getLocalAABB().getFullBox();
		std::shared_ptr<vertexVector> bBox = bObj->getLocalAABB().getFullBox();
		std::shared_ptr<vertexVector> aBoxWorld = translateVertexVector(obj.getObject(a)->getRenderableComponent()->getProjectionMatrix(), aBox);
		std::shared_ptr<vertexVector> bBoxWorld = translateVertexVector(obj.getObject(b)->getRenderableComponent()->getProjectionMatrix(), bBox);
		
		// Generate planes to check to see if they are seperated by that plane
		for (size_t i = 0; i < 3; i++){
			// Generate the normals of each plane
			if (sat(glm::normalize(aBoxWorld->at(0) - aBoxWorld->at(1 << i)), aObj, bObj, aBoxWorld, bBoxWorld)){
				// If they are separated, the objects don't collide
				return false;
			}
			// Generate the planes of each face
			for (size_t j = 0; j < 3; j++){
				vec3 vec = glm::cross(glm::normalize(aBoxWorld->at(0) - aBoxWorld->at(1 << i)), glm::normalize(bBoxWorld->at(0) - bBoxWorld->at(1 << j)));
				if (sat(vec, aObj, bObj, aBoxWorld, bBoxWorld)){
					return false;
				}
			}
			if (sat(glm::normalize(bBoxWorld->at(0) - bBoxWorld->at(1 << i)), aObj, bObj, aBoxWorld, bBoxWorld)){
				return false;
			}
		}
		return true;
	}

	// Return true if the plane specified by the vec3 parts the two PhysicsObjects
	// Verbosely: If there exists a(n arbitrary) plane which has normal v 
	// which separates the two objects, return true
	bool sat(const vec3 & v, std::shared_ptr<PhysicsObject> physA, std::shared_ptr<PhysicsObject> physB, std::shared_ptr<vertexVector> aBoxWorld, std::shared_ptr<vertexVector> bBoxWorld){
		// Note the projection of a onto b is (a.b/b.b)b -> which can be simplifiedto test only a.b
		float maxA, maxB, minA, minB;
		float a = glm::dot(v, aBoxWorld->at(0));
		float b = glm::dot(v, bBoxWorld->at(0));
		minA = a;
		maxA = a;
		minB = b;
		maxB = b;
		for (size_t i = 1; i < 8; i++){
			a = glm::dot(v, aBoxWorld->at(i));
			if (a > maxA){
				maxA = a;
			}
			else if (a < minA){
				minA = a;
			}
			b = glm::dot(v, bBoxWorld->at(i));
			if (b > maxA){
				maxB = b;
			}
			else if (b < minA){
				minB = b;
			}
		}
		// If the projections onto the axis don't overlap
		return (maxA < minB || maxB < minA);
	}

	void reversePlayer(const GameObjectID id){
		GameObject obj = *ObjectManager::getInstance().getObject(id);
		PhysicsObject phys = *obj.getPhysicsComponent();
		vec3 dir = phys.getOrientation();
		float speed = glm::length(dir);
		vec3 A = phys.getA();
		// TODO: Adjust arbitrary constant according to playtesting
		phys.setA(A + glm::normalize(dir)*(-2-speed));
	}

	// TODO: Make into generic spin function
	void turnLeft(const GameObjectID id){
		GameObject obj = *ObjectManager::getInstance().getObject(id);
		std::shared_ptr<PhysicsObject> phys = obj.getPhysicsComponent();
		turnObject(phys, Quaternion(0.02f, 0, 1, 0), &PhysicsObject::getOrientation, &PhysicsObject::setOrientation);
		turnObject(phys, Quaternion(0.02f, 0, 1, 0), &PhysicsObject::getV, &PhysicsObject::setV);

	}

	void turnObject(std::shared_ptr<PhysicsObject> phys, Quaternion rotator, const vec3 (PhysicsObject::*getter) () const, void (PhysicsObject::*setter) (vec3 &)){
		vec3 dir = (*phys.*getter)();
		Quaternion oldVector = Quaternion(0, dir.x, dir.y, dir.z);
		Quaternion out = rotator*oldVector;
		vec3 updated = vec3(out.x, out.y, out.z);
		(*phys.*setter)(updated);
	}
}
