#include <algorithm>
#include <iostream>

#include "../Common.hpp"
#include "../GameObject.hpp"
#include "../Player.hpp"
#include "../LocationComponent.hpp"
#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "../UpdateManager.hpp"

namespace PhysicsMaths{
	// Tests for global-axis aligned bounding box collision.
	// @param a,b - the two PhysicsObject items to check with
	// @return true if they collide
	bool simpleCollision(const GameObjectGlobalID a, const GameObjectGlobalID b){
		UpdateManager& update = UpdateManager::getInstance();
		auto state = update.getState();
		GameObject aObj = *state.at(a);
		GameObject bObj = *state.at(b);
		return simpleCollision(*aObj.physComp->getWorldAABB(), *bObj.physComp->getWorldAABB());
	}

	// Tests for global-axis aligned bounding box collision.
	// @param a,b - the two AABB items to check with
	// @return true if they collide
	bool simpleCollision(const AABB & a, const AABB & b){
		vec3 vec3Difference = a.getCenter() - b.getCenter();
		
		// If there exists a gap in the direction of a principal axis, then
		// there exists a plane normal to that axis which separates them.
		float xGap = abs(vec3Difference.x) - (a.getXSpan() + b.getXSpan()) * 0.5f;
		if (xGap >= 0.0f){
			return false;
		}
		float yGap = abs(vec3Difference.y) - (a.getYSpan() + b.getYSpan()) * 0.5f;
		if (yGap >= 0.0f){
			return false;
		}
		float zGap = abs(vec3Difference.z) - (a.getZSpan() + b.getZSpan()) * 0.5f;
		if (zGap >= 0.0f){
			return false;
		}
		return true;
	}

	void handleCollision(GameObjectGlobalID aID, GameObjectGlobalID bID, std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> & map){
		GameObject objA = *((map.find(aID))->second);
		GameObject objB = *((map.find(bID))->second);
		handleCollision(objA, objB);
	}

	void handleCollision(GameObject & objA, GameObject & objB){

		std::shared_ptr<PhysicsObject> physA = objA.physComp;
		std::shared_ptr<PhysicsObject> physB = objB.physComp;
		vec3 aCen = physA->getLocalAABB().getCenter();
		vec3 bCen = physB->getLocalAABB().getCenter();

		// Transform these to world space
		glm::mat4 matA = glm::translate(glm::mat4x4(1.0f), objA.locComp->getPosition());
		glm::mat4 matB = glm::translate(glm::mat4x4(1.0f), objB.locComp->getPosition());
		aCen = vec3(matA * glm::vec4(aCen.x, aCen.y, aCen.z, 1.0f));
		bCen = vec3(matB * glm::vec4(bCen.x, bCen.y, bCen.z, 1.0f));

		// Calculate relative velocity and position
		if (aCen == bCen) return;
		vec3 sDiff = aCen - bCen;
		vec3 sDiffNormal = glm::normalize(sDiff);
		vec3 vDiff = physA->getV() - physB->getV();

		// Calculate relative velocity along normal direction
		float speedAlongCollisionNormal = glm::dot(vDiff, sDiffNormal);
		bool seperatingMetric = glm::length(sDiff) > glm::length(vDiff - sDiff);

		// Do not resolve if they are separating already
		if (seperatingMetric){
			//std::cout << "Discarded collision\n";
			return;
		}

		// Choose minimal restitution
		float e = ((physA->getRest() < physB->getRest()) ? physA : physB)->getRest();

		float m1 = physA->getMass();
		float m2 = physB->getMass();

		vec3 u1 = glm::dot(physA->getV(), sDiffNormal) * sDiffNormal;
		vec3 u2 = glm::dot(physB->getV(), sDiffNormal) * sDiffNormal;

		// Scoring
		if (objA.userControllable && objB.userControllable) {
			Player playerA = (Player&) objA;
			Player playerB = (Player&) objB;
			float speed_diff = glm::length(physA->getV()) - glm::length(physB->getV());
			// increase score of fastest player
			// TODO: score increase to depend on difference in speeds
			if (speed_diff > 0) {
				playerA.modifyScore(+1);
			} else {
				playerB.modifyScore(+1);
			}
		}

		vec3 u1rejection = physA->getV() - u1;
		vec3 u2rejection = physB->getV() - u2;

		physA->setX(physA->getX() - 0.5f * (sDiffNormal - vec3(0, sDiffNormal.y, 0)));
		physB->setX(physB->getX() + 0.5f * (sDiffNormal - vec3(0, sDiffNormal.y, 0)));

		if (m1 == 0 && m2 == 0) return;

		if (m1 == 0){
			vec3 newV = -e * u2;
			physB->setV(newV);
			return;
		}
		if (m2 == 0){
			vec3 newV = -e * u1;

			physA->setV(newV);
			return;
		}

		vec3 v1 = (m1 * u1 + m2 * (u2 - e * speedAlongCollisionNormal * sDiffNormal)) / (m1 + m2);
		vec3 v2 = v1 + e * speedAlongCollisionNormal * sDiffNormal;
		vec3 newVA = u1rejection + v1, newVB = u2rejection + v2;
		newVA.y = 0.0f;
		newVB.y = 0.0f;
		physA->setV(newVA);
		physB->setV(newVB);
	}

	std::vector<glm::vec3> convertGLfloatToVec3(const std::vector<GLfloat> & data) {
		std::vector<glm::vec3> newData;
		
		for (size_t i = 0; i < data.size(); i+=3) {
			newData.push_back(glm::vec3(data.at(i), data.at(i + 1), data.at(i + 2)));
		}

		return newData;
	}

	glm::vec3 translateVertex(const glm::mat4x4 & matrix, const vec3 & vector){
		return vec3(matrix * glm::vec4(vector.x, vector.y, vector.z, 1.0f));
	}

	std::shared_ptr<vertexVector> translateVertexVector(const glm::mat4x4 & matrix, const std::shared_ptr<vertexVector> & vertices) {
		std::shared_ptr<vertexVector> result = std::make_shared<vertexVector>(*vertices);
		for (size_t i = 0; i < vertices->size(); i++){
			result->at(i) = translateVertex(matrix, result->at(i));
		}
		return result;
	}
}
