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

		// TODO: Requires handling of complex collision
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
		physObj.setV(UATtoV(physObj.getV(), physObj.getA(), timestep));
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

}
