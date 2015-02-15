#include "Chunk.hpp"

#include "Common.hpp"
#include "Cube.hpp"
#include "LocationComponent.hpp"
#include "RenderableComponent.hpp"
#include "shader.hpp"
#include "SimplexNoise.hpp"
#include "Texture.hpp"

#include <iostream>
#include <memory>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
Chunk::Chunk()
	: Chunk(0.0, 0.0, 0.0)
{

}

Chunk::Chunk(double xPos, double yPos, double zPos) {
	locationComponent->setPosition(glm::vec3(xPos, yPos, zPos));

	init();
}

void Chunk::init() {
	//Init starting renderable data 
	std::shared_ptr<Texture> texture = std::make_shared<Texture>("resources/textures/grass.png");
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("resources//shaders//default_shader");
	if (!shader->isLoaded()) {
		std::cerr << "ERROR: In Chunk::init, cannot load shader." << std::endl;
		return;
	}

	renderableComponent->setShader(shader);
	renderableComponent->setTexture(texture);


	//The number of cubes we actually want to render
	int numCubes = 0;

	//NOTE: Cube[] is 1000 in header, need to update
	int xLength = 30, yLength = 30, zLength = 30;
	SimplexNoise noiseGenerator;

	//Build up our world
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			for (int y = -yLength / 2; y < yLength / 2; ++y) {

				double noise = noiseGenerator.noise(x, y, z);
				if (noise > 0.4) {
					//Don't  draw it => Don't generate its geometry
					continue;
				}

				//TODO optmise, only draw if visible
				float sf = 8.0f;
				float xPos = sf*(x);
				float yPos = sf*(y);
				float zPos = sf*(z);

				//TODO improve efficiency of this entire class
				for (size_t i = 0; i < sizeof(cubeData) / sizeof(glm::vec3); ++i) {
					//div by 2 as cube data is -1 to +1
					glm::vec3 newCubeData = cubeData[i] * 4.0f;
					newCubeData.x += xPos;
					newCubeData.y += yPos;
					newCubeData.z += zPos;
					chunkVertexData.push_back(newCubeData.x);
					chunkVertexData.push_back(newCubeData.y);
					chunkVertexData.push_back(newCubeData.z);
				}

				for (size_t i = 0; i < sizeof(cubeColours) / sizeof(GLfloat); ++i) {
					chunkColourData.push_back(cubeColours[i]);
				}

				for (size_t i = 0; i < sizeof(cubeTextureCoords) / sizeof(GLfloat); ++i) {
					chunkTextureCoordsData.push_back(cubeTextureCoords[i]);
				}

				numCubes++;
			}
		}
	}

	//set renderable data
	renderableComponent->setVertexData(chunkVertexData, false);
	renderableComponent->setNumVerticesRender(numCubes * 36);
	renderableComponent->setColourData(chunkColourData, false);
	renderableComponent->setTextureCoordsData(chunkTextureCoordsData, false);

}

const glm::vec3 Chunk::cubeData[36] = {
	glm::vec3(-1.0f, -1.0f, -1.0f), // triangle 1 : begin
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end
	glm::vec3(1.0f, 1.0f, -1.0f), // triangle 2 : begin
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f), // triangle 2 : end
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),

	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),

	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f)
};

const GLfloat Chunk::cubeColours[4 * 36] = {
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

const GLfloat Chunk::cubeTextureCoords[2 * 36] = {
	1.0f, 1.0f,// triangle 1 : begin
	0.5f, 1.0f,
	1.0f, 0.5f, // triangle 1 : end
	1.0f, 1.0f, // triangle 2 : begin
	1.0f, 1.0f,
	1.0f, 1.0f, // triangle 2 : end
	1.0f, 0.5f,
	1.0f, 1.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	0.5f, 1.0f,
	1.0f, 1.0f,

	0.5f, 1.0f,
	0.5f, 1.0f,
	0.5f, 1.0f,
	0.5f, 1.0f,
	0.5f, 1.0f,
	0.5f, 1.0f,
	0.5f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.5f, 1.0f,
	1.0f, 0.5f,
	1.0f, 0.5f,
	1.0f, 1.0f,
	1.0f, 0.5f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	0.5f, 0.5f,
	1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,
};