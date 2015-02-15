#include "Game.hpp"
#include "Texture.hpp"
#include "../src/CommonMinimal.hpp"
#include "LocationComponent.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <time.h>
#include "SimplexNoise.hpp"
void Game::init() {
	cubes = new Cube[numCubes];
	std::shared_ptr<Texture> texture = std::make_shared<Texture>("resources/textures/grass.png");
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("resources//shaders//default_shader");
	std::shared_ptr<RenderableComponent> renderableComponent = std::make_shared<RenderableComponent>();
	renderableComponent->setShader(shader);
	renderableComponent->setTexture(texture);
	renderableComponent->setVertexData(cubes[0].cubeVertexData, false);
	renderableComponent->setNumVerticesRender(36);
	renderableComponent->setColourData(cubes[0].cubeColourData, false);
	renderableComponent->setTextureCoordsData(cubes[0].cubeTextureCoordsData, false);

	for (int i = 0; i < numCubes; i++)
		cubes[i].setRenderableComponent(renderableComponent);

	float xPosCenter = 0.0f, yPosCenter = -10.0f, zPosCenter = -200.0f;
	//NOTE: Cube[] is 1000 in header, need to update
	int xLength = 50, yLength = 50, zLength = 50;
	SimplexNoise noiseGenerator;
	//Build up our world
	int i = 0;
	for (int x = -xLength / 2; x < xLength/2; ++x) {
		for (int z = -zLength / 2; z < zLength/2; ++z) {
			for (int y = -yLength / 2; y < yLength/2; ++y) {
				
				std::shared_ptr<LocationComponent> locationComponent = cubes[i].getLocationComponent();
				double noise = noiseGenerator.noise(x, y, z);
				if (noise > 0.4) {
					cubes[i].setVisible(false);
					continue;
				}
				//todo optmise, only draw if visible
				float sf = 8.0f;
				float xPos = sf*(x) + xPosCenter;
				float yPos = sf*(y) + yPosCenter;
				float zPos = sf*(z) + zPosCenter;

				locationComponent->setPosition(glm::vec3(xPos, yPos, zPos));

				if (!shader->isLoaded())
					exit(0);

				++i;
			}
		}
	}
}

void Game::renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) {
	//Move camera to the position of the player
	float xPos = 0.0f, yPos = -10.0f, zPos = -200.0f;
	glm::mat4 baseModelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(xPos, yPos, zPos));

	//per cube
	for (int i = 0; i < numCubes; ++i) {
		if (!(cubes[i].isVisible() && cubes[i].isRenderable()))
			continue;

		std::shared_ptr<LocationComponent> locationComponent = cubes[i].getLocationComponent();
		glm::vec3 pos = locationComponent->getPosition();

		modelViewMatrix = glm::translate(baseModelViewMatrix, glm::vec3(pos.x, pos.y, pos.z));

		//Get the renderable component and bind in the shader
		std::shared_ptr<RenderableComponent> renderableComponent = cubes[i].getRenderableComponent();
		std::shared_ptr<Shader> objectShader = renderableComponent->getShader();

		if (!objectShader)
			continue;

		renderableComponent->bindShader();

		renderableComponent->setProjectionMatrix(projectionMatrix);
		renderableComponent->setModelviewMatrix(modelViewMatrix);

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

		//draw arrays
		//TODO add switch to allow index drawing
		glDrawArrays(GL_TRIANGLES, 0, renderableComponent->getNumVerticesRender());

		//Release vertex, textures and shaders
		renderableComponent->releaseTextures();
		renderableComponent->releaseVBOs();
		renderableComponent->releaseShader();

	}
}