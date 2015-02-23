#include "Chunk.hpp"
#include "Game.hpp"
#include "Texture.hpp"
#include "../src/Common.hpp"
#include "LocationComponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include <string>
#include "Player.hpp"
#include "ObjectManager.hpp"
#include "SimplexNoise.hpp"


void Game::init() {
	//Sahil: You can comment this out
	chunks = new Chunk[numChunks];

	//Add objects to the object manager
	ObjectManager& objManager = ObjectManager::getInstance();
	for (int i = 0; i < numChunks; ++i) {
		objManager.addObject(std::make_shared<Chunk>(chunks[i]));
	}
	float xPos = 0.0f, yPos = 50.0f, zPos = -60.0;
	player = std::make_shared<Player>(xPos, yPos, zPos);
	objManager.addObject(player);

}

void Game::renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) {

	//Render all objects
	ObjectManager& objManager = ObjectManager::getInstance();
	std::vector<GameObjectID> objects = objManager.getObjects();
	for (GameObjectID objectID : objects) {
		std::shared_ptr<GameObject> objectPtr = objManager.getObject(objectID);
		if (!(objectPtr->isVisible() && objectPtr->isRenderable()))
			continue;

		std::shared_ptr<LocationComponent> locationComponent = objectPtr->getLocationComponent();
		glm::vec3 objPos = locationComponent->getPosition();
	
		//Move object to world space 
		glm::mat4 objWorldMatrix = glm::translate(glm::mat4(1.0), glm::vec3(objPos.x, objPos.y,objPos.z));

		//Move camera to the position of the player
		glm::vec3 playerPos = player->getLocationComponent()->getPosition();
		glm::vec3 eyePos(playerPos.x, playerPos.y, playerPos.z);
		glm::mat4 objCameraMatrix = glm::translate(objWorldMatrix, glm::vec3(-eyePos.x, -eyePos.y, -eyePos.z));
		objCameraMatrix = modelViewMatrix * objCameraMatrix;
		std::cout << playerPos.x << " " << playerPos.y << " " << playerPos.z << std::endl;
		//Get the renderable component and bind in the shader
		std::shared_ptr<RenderableComponent> renderableComponent = objectPtr->getRenderableComponent();
		std::shared_ptr<Shader> objectShader = renderableComponent->getShader();
 
		if (!objectShader)
			continue;

		renderableComponent->bindShader();

		renderableComponent->setProjectionMatrix(projectionMatrix);
		renderableComponent->setModelviewMatrix(objCameraMatrix);

		//set uniforms
		glUniformMatrix4fv(glGetUniformLocation(objectShader->getProgram(), "matProjection"),
			1,
			GL_FALSE,
			glm::value_ptr(renderableComponent->getProjectionMatrix()));

		glUniformMatrix4fv(glGetUniformLocation(objectShader->getProgram(), "matModelView"),
			1,
			GL_FALSE,
			glm::value_ptr(renderableComponent->getModelviewMatrix()));

		//Bind VBOs
		renderableComponent->bindVBOs();

		//Bind textures
		renderableComponent->bindTextures();

		//If we need to use our index buffer
		if (renderableComponent->usesIndexedVBO()) {
			//TODO: Consider using unsigned short rather than unsigned short for memory conservation
			glDrawElements(
				GL_TRIANGLES,
				renderableComponent->getIndexDataSize(),
				GL_UNSIGNED_INT,
				nullptr);
		}
		else{
			//draw arrays
			glDrawArrays(GL_TRIANGLES, 0, renderableComponent->getNumVerticesRender());
		}

		//Release vertex, textures and shaders
		renderableComponent->releaseTextures();
		renderableComponent->releaseVBOs();
		renderableComponent->releaseShader();
		//Draw blue text at screen coordinates(100, 120), where(0, 0) is the top - left of the
			// screen in an 18-point Helvetica font

	}
}