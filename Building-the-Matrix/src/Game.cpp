#include "Game.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <memory>
#include <stdlib.h>
#include <time.h>
void Game::init() {


	///
	/// Test data 
	///

	/**
	 * The vertices are needed in a counter-clockwise facing orientation
	 *   6 --- 7
	 *  /|    /|
	 * 2 --- 3 |
	 * | 4 --|-5
	 * |/    |/
	 * 0 --- 1
	 *
	 *Faces:
	 *
	 *
	 * 2---3
	 * |   |
	 * |   |
	 * 0---1
	 * is then
	 *
	 * 2---3
	 * |  /
	 * | /
	 * 0
	 * 0-3-2
	 *     3
	 *   / |
	 *  /  |
	 * 0 --1
	 * 0-1-3
	 */
	GLfloat cubeData[] = {
		-1.0f, -1.0f, -1.0f, // triangle 1 : begin
		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, -1.0f, // triangle 2 : begin
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f, // triangle 2 : end
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	GLfloat cubeColours[] = {
		1.0f, 1.0f, 1.0f, 1.0f,// triangle 1 : begin
		0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 1.0f, 1.0f, // triangle 1 : end
		1.0f, 1.0f, 0.5f, 1.0f, // triangle 2 : begin
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f, // triangle 2 : end
		1.0f, 0.5f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 1.0f, 1.0f,
		1.0f, 0.5f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.5f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.5f, 1.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.5f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	GLfloat* cubeVertexData = new GLfloat[sizeof(cubeData) / sizeof(GLfloat)];
	GLfloat* cubeColourData = new GLfloat[sizeof(cubeColours) / sizeof(GLfloat)];
	for (size_t i = 0; i < sizeof(cubeData) / sizeof(GLfloat); ++i) {
		*(cubeVertexData + i) = 10.0f*cubeData[i];
	}
	for (size_t i = 0; i < sizeof(cubeColours) / sizeof(GLfloat); ++i) {
		*(cubeColourData + i) = cubeColours[i];
	}

	for (int i = 0; i < numCubes; ++i) {
		cubes[i] = new RenderableComponent();
		std::shared_ptr<Shader> shader = std::make_shared<Shader>("resources//shaders//default_shader");

		if (!shader->isLoaded())
			exit(0);

		cubes[i]->setShader(shader);
		cubes[i]->setVertexData(cubeVertexData, sizeof(cubeData) / sizeof(GLfloat), false);
		cubes[i]->setNumVerticesRender((sizeof(cubeData) / sizeof(GLfloat)) / 3);
		cubes[i]->setColourData(cubeColourData, sizeof(cubeColours) / sizeof(GLfloat), false);
	}
}

void Game::renderScene(glm::mat4 modelViewMatrix, glm::mat4 projectionMatrix) {
	
	for (int i = 0; i < numCubes; ++i) {
		float xPos = 0.0f, yPos = -10.0f, zPos = -50.0f;

		//Move camera to the position of the player
		modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(xPos, yPos, zPos));

		//Get the renderable component and bind in the shader
		RenderableComponent* renderableComponent = cubes[i];
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