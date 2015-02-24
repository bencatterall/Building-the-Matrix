#include <algorithm>

#include "../Common.hpp"
#include "../GameObject.hpp"
#include "../Player.hpp"
#include "../LocationComponent.hpp"
#include "AABB.hpp"
#include "PhysicsMaths.hpp"
#include "PhysicsObject.hpp"
#include "../UpdateManager.hpp"


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
	bool simpleCollision(const GameObjectGlobalID a, const GameObjectGlobalID b){
		//TODO: Call proper instance
		UpdateManager& update = UpdateManager::getInstance();
		auto state = update.getState();
		GameObject aObj = *state.at(a);
		GameObject bObj = *state.at(b);
		return simpleCollision(*aObj.physComp, *bObj.physComp);
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
		vec3 vec3Difference = a.getCenter() - b.getCenter();
		
		// If there exists a gap in the direction of a principal axis, then
		// there exists a plane normal to that axis which separates them.
		float xGap = vec3Difference.x - (a.getXSpan() + b.getXSpan()) * 0.5f;
		if (xGap > 0.0f){
			return false;
		}
		float yGap = vec3Difference.y - (a.getYSpan() + b.getYSpan()) * 0.5f;
		if (yGap > 0.0f){
			return false;
		}
		float zGap = vec3Difference.z - (a.getZSpan() + b.getZSpan()) * 0.5f;
		if (zGap > 0.0f){
			return false;
		}
		return true;
	}

	/*void handleCollision(GameObjectGlobalID aID, GameObjectGlobalID bID){
		UpdateManager& objMan = UpdateManager::getInstance();
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> map = objMan.getState();
		GameObject objA = *objMan.getGameObject(aID);
		GameObject objB = *objMan.getGameObject(bID);
		handleCollision(objA, objB);
	}*/

	void handleCollision(GameObjectGlobalID aID, GameObjectGlobalID bID, std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> map){
		GameObject objA = *((map.find(aID))->second);
		GameObject objB = *((map.find(bID))->second);
		handleCollision(objA, objB);
	}

	void handleCollision(GameObject& objA, GameObject& objB){

		PhysicsObject physA = *objA.physComp;
		PhysicsObject physB = *objB.physComp;
		vec3 aCen = physA.getLocalAABB().getCenter();
		vec3 bCen = physB.getLocalAABB().getCenter();

		// Transform these to world space
		glm::mat4 matA = glm::translate(glm::mat4x4(1.0f), objA.locComp->getPosition());
		glm::mat4 matB = glm::translate(glm::mat4x4(1.0f), objB.locComp->getPosition());
		aCen = vec3(matA * glm::vec4(aCen.x, aCen.y, aCen.z, 1.0f));
		bCen = vec3(matB * glm::vec4(bCen.x, bCen.y, bCen.z, 1.0f));

		// Calculate relative velocity and position
		if (aCen == bCen) return;
		vec3 sDiff = aCen - bCen;
		vec3 sDiffNormal = glm::normalize(sDiff);
		vec3 vDiff = physA.getV() - physB.getV();

		// Calculate relative velocity along normal direction
		float velDelAlongCollisionNormal = glm::dot(vDiff, sDiffNormal);

		// Do not resolve if they are separating already
		if (velDelAlongCollisionNormal > 0.0f){
			return;
		}
		// Choose minimal restitution
		float e = ((physA.getRest() < physB.getRest()) ? physA : physB).getRest();

		float m1 = physA.getMass();
		float m2 = physB.getMass();

		vec3 u1 = glm::dot(physA.getV(), sDiffNormal) * sDiffNormal;
		vec3 u2 = glm::dot(physB.getV(), sDiffNormal) * sDiffNormal;

		// Scoring
		if (objA.userControllable && objB.userControllable) {
			Player playerA = (Player&) objA;
			Player playerB = (Player&) objB;
			float speed_diff = glm::length(physA.getV()) - glm::length(physB.getV());
			// increase score of fastest player
			// TODO: score increase to depend on difference in speeds
			if (speed_diff > 0) {
				playerA.modifyScore(+1);
			} else {
				playerB.modifyScore(+1);
			}
		}

		vec3 u1rejection = physA.getV() - u1;
		vec3 u2rejection = physB.getV() - u2;

		if (m1 == 0 && m2 == 0) return;

		if (m1 == 0){
			physB.setV(-e * u2);
			return;
		}
		if (m2 == 0){
			physA.setV(-e * u1);
			return;
		}

		vec3 v1 = (m1 * u1 + m2 * (u2 - e * velDelAlongCollisionNormal * sDiffNormal)) / (m1 + m2);
		vec3 v2 = v1 + e * velDelAlongCollisionNormal * sDiffNormal;

		physA.setV(u1rejection + v1);
		physB.setV(u2rejection + v2);
	}

	void stepObject(std::shared_ptr<PhysicsObject> physObj, float timestep){
		vec3 delS = UATtoS(physObj->getV(), physObj->getA(), timestep);
		physObj->setX(physObj->getX() + delS);
		vec3 newV = UATtoV(physObj->getV(), physObj->getA(), timestep);
#ifdef COMPLEX_PHYSICS
		vec3 acc = physObj->getA();
		// Drag calculations
		float newVLength = glm::length(newV);
		// If we are speeding up
		if (glm::dot(acc, newV) > 0){
			acc = acc - (physObj->getQuadDrag()*newVLength + physObj->getLinDrag()) * 1.0f * newV;
			if (glm::dot(acc, newV) < 0){
				physObj->setA(vec3());
			}
			else{
				physObj->setA(acc);
			}
		}
		else{
			// If we are slowing down
			if (glm::dot(acc, newV) < 0){
				acc = acc + (physObj->getQuadDrag()*newVLength + physObj->getLinDrag()) * 1.0f * newV;
				physObj->setA(vec3());
			}
		}
#endif //COMPLEX_PHYSICS
		physObj->setV(newV);
	}



	std::vector<glm::vec3> convertGLfloatToVec3(std::vector<GLfloat> data) {
		std::vector<glm::vec3> newData;

		for (size_t i = 0; i < data.size(); i+=3) {
			newData.push_back(glm::vec3(data.at(i), data.at(i + 1), data.at(i + 2)));
		}

		return newData;
	}

	const glm::vec3 translateVertex(const glm::mat4x4 & matrix, const vec3 vector){
		return vec3(matrix * glm::vec4(vector.x, vector.y, vector.z, 1.0f));
	}

	const std::shared_ptr<vertexVector> translateVertexVector(const glm::mat4x4 & matrix, const std::shared_ptr<vertexVector> vertices) {
		std::shared_ptr<vertexVector> result = std::make_shared<vertexVector>(*vertices);
		for (size_t i = 0; i < vertices->size(); i++){
			result->at(i) = translateVertex(matrix, result->at(i));
		}
		return result;
	}

	bool complexCollision(const GameObjectGlobalID a, const GameObjectGlobalID b){
		UpdateManager & obj = UpdateManager::getInstance();
		std::map<GameObjectGlobalID, std::shared_ptr<GameObject>> state = obj.getState();
		GameObject aObj = *state.at(a);
		GameObject bObj = *state.at(b);
		return complexCollision(aObj, bObj);
	}

	bool complexCollision(const GameObject & objectA, const GameObject & objectB){
		std::shared_ptr<PhysicsObject> aObj = objectA.physComp;
		std::shared_ptr<PhysicsObject> bObj = objectB.physComp;
		std::shared_ptr<vertexVector> aBox = aObj->getLocalAABB().getFullBox();
		std::shared_ptr<vertexVector> bBox = bObj->getLocalAABB().getFullBox();
		glm::mat4x4 aTrans = glm::translate(glm::mat4x4(1.0f), aObj->getX());
		glm::mat4x4 bTrans = glm::translate(glm::mat4x4(1.0f), bObj->getX());
		aTrans *= objectA.locComp->getRotationMatrix();
		bTrans *= objectB.locComp->getRotationMatrix();
		std::shared_ptr<vertexVector> aBoxWorld = PhysicsMaths::translateVertexVector(aTrans, aBox);
		std::shared_ptr<vertexVector> bBoxWorld = PhysicsMaths::translateVertexVector(bTrans, bBox);
		
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

	void acceleratePlayer(std::shared_ptr<PhysicsObject> phys){
#ifdef COMPLEX_PHYSICS
		vec3 dir = phys->getOrientation();
		float speed = glm::length(phys->getV());
		vec3 A = phys->getA();
		// TODO: Consider further mechanisms for determining power
		// TODO: Adjust arbitrary constant according to playtesting
		phys->setA(A + dir*(5.0f - speed));
#else
		vec3 dir = phys->getOrientation();
		vec3 A = phys->getA(), V = phys->getV();
		if (glm::dot(dir, V) > 0) { // go faster
			if (glm::length(V) > 5){ //max speed
				phys->setA(vec3());
			}
			else{ //speed up
				vec3 newA = A - dir;
				if (newA != vec3()){
					newA = glm::normalize(A - dir);
				}
				phys->setA(newA);
			}
		}
		else{
			vec3 newA = A + dir;
			if (newA != vec3()){
				newA = glm::normalize(newA);
			}
			phys->setA(newA);
		}
#endif
	}


	void reversePlayer(std::shared_ptr<PhysicsObject> phys){
#ifdef COMPLEX_PHYSICS
		vec3 dir = phys->getOrientation();
		float speed = glm::length(phys->getV());
		vec3 A = phys->getA();
		// TODO: Adjust arbitrary constant according to playtesting
		phys->setA(A + dir*(-2.0f-speed));
#else
vec3 dir = phys->getOrientation();
vec3 A = phys->getA(), V = phys->getV();
if (glm::dot(dir, V) < 0) { // go faster
	if (glm::length(V) > 5){ //max speed
		phys->setA(vec3());
	}
	else{ //speed up
		vec3 newA = A - dir;
		if (newA != vec3()){
			newA = glm::normalize(A - dir);
		}
		phys->setA(newA);
	}
}
else{
	vec3 newA = A - dir;
	if (newA != vec3()){
		newA = glm::normalize(A - dir);
	}
	phys->setA(newA);
	
}
#endif
	}

	void turnLeft(std::shared_ptr<PhysicsObject> phys, float turnSpeed){
		turnObject(phys, Quaternion(turnSpeed, 0.0f, 1.0f, 0.0f), &PhysicsObject::getOrientation, &PhysicsObject::setOrientation);
		turnObject(phys, Quaternion(turnSpeed, 0.0f, 1.0f, 0.0f), &PhysicsObject::getV, &PhysicsObject::setV);

	}

	void turnRight(std::shared_ptr<PhysicsObject> phys, float turnSpeed){
		turnLeft(phys, -turnSpeed);
	}

	void turnObject(std::shared_ptr<PhysicsObject> phys, Quaternion rotator, const vec3 (PhysicsObject::*getter) () const, void (PhysicsObject::*setter) (vec3 &)){
		vec3 dir = (*phys.*getter)();
		Quaternion oldVector = Quaternion(0.0f, dir.x, dir.y, dir.z);
		Quaternion out = rotator*oldVector;
		vec3 updated = vec3(out.x, out.y, out.z);
		(*phys.*setter)(updated);
	}

	vec3 convertYPRtoDirection(const vec3 YPR){
		float xRad = YPR.x * PI / 180.0f, yRad = YPR.y * PI / 180.0f;
		return vec3(
			cos(xRad) * cos(yRad),
			sin(xRad) * cos(yRad),
			sin(yRad)
			);
	}
}
