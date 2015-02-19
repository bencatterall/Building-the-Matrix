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
	int xLength = 256, yLength = 150, zLength = 256;
	SimplexNoise noiseGenerator;

	//Generate the world 
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
				//Generate noise 
				//[-1.0,1.0]
			double noise = (noiseGenerator.noise2D(x / 80.0, z / 80.0));

				/*
				if (noise > 0.1) {
					//Don't  draw it => Don't generate its geometry
					chunkData[x + (xLength / 2)][y + (yLength / 2)][z + (zLength / 2)] = 0;
					continue; 
				}
				*/
				

				//fill in y below
				int yHeight = (int)(noise*(yLength/2) * 0.3);
				for (int y = 0; y < yHeight; ++y) {
					chunkData[x + (xLength / 2)][y+yLength /2][z + (zLength / 2)] = 1;
					numCubes++;
				}
				for (int y = yHeight; y < yLength; ++y) {
					chunkData[x + (xLength / 2)][y + yLength / 2][z + (zLength / 2)] = 0;
				}
				//Set data
				chunkNoiseData[x + (xLength / 2)][yHeight + yLength / 2][z + (zLength / 2)] = noise;
				//Set to default grass
				//chunkData[x + (xLength / 2)][yHeight][z + (zLength / 2)] = 1;
				//numCubes++;
		}
	}

	//Optimise and clean up the world
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			for (int y = -yLength / 2; y < yLength / 2; ++y) {
				//perform clustering
				int baseX = x + (xLength / 2);
				int baseY = y + (yLength / 2);
				int baseZ = z + (zLength / 2);

				//If our cube has no entry
				if (chunkData[baseX][baseY][baseZ] == 0) {
					chunkVisibleData[baseX][baseY][baseZ] = false;
					continue;
				}

				chunkVisibleData[baseX][baseY][baseZ] = false;
				//hide cubes if there is a cube:
				//above and below
				//in front and behind
				//to the left and the right
				if (baseX - 1 > 0) {
					if (chunkData[baseX - 1][baseY][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseX + 1 < xLength) {
					if (chunkData[baseX + 1][baseY][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseY - 1 > 0) {
					if (chunkData[baseX][baseY - 1][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseY + 1 < yLength) {
					if (chunkData[baseX][baseY + 1][baseZ] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseZ - 1 > 0)  {
					if (chunkData[baseX][baseY][baseZ - 1] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				if (baseZ + 1 < zLength) {
					if (chunkData[baseX][baseY][baseZ + 1] == 0) {
						chunkVisibleData[baseX][baseY][baseZ] = true;
						continue;
					}
				}
				
			}
		}
	}

	//The six face
	//Defined from looking at the world down -z, with x and y aligned to screen
//	chunkData[0]; //left
//	chunkData[1]; // right
	chunkData[2]; // top
	chunkData[3]; //bottom
	chunkData[4]; //front
	chunkData[5]; //back

	//Generate renderable data[0]
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			for (int y = -yLength / 2; y < yLength / 2; ++y) {

				int baseX = x + (xLength / 2);
				int baseY = y + (yLength / 2);
				int baseZ = z + (zLength / 2);

				//if not visible
				if (!chunkVisibleData[baseX][baseY][baseZ])
					continue;
			
				float sf = 2.0f;
				float xPos = sf*(x);
				float yPos = sf*(y);
				float zPos = sf*(z);

				//generate face data

				//TODO improve efficiency of this entire class
				for (size_t i = 0; i < sizeof(cubeData) / sizeof(glm::vec3); ++i) {
					//div by 2 as cube data is -1 to +1
					glm::vec3 newCubeData = cubeData[i] * 1.0f;
					newCubeData.x += xPos;
					newCubeData.y += yPos;
					newCubeData.z += zPos;
					chunkVertexData.push_back(newCubeData.x);
					chunkVertexData.push_back(newCubeData.y);
					chunkVertexData.push_back(newCubeData.z);
				}

			/*	for (size_t i = 0; i < sizeof(cubeColours) / sizeof(GLfloat); ++i) {
					chunkColourData.push_back(cubeColours[i]);
				}
				*/
				for (size_t i = 0; i < sizeof(cubeTextureCoords) / sizeof(GLfloat); ++i) {
					chunkTextureCoordsData.push_back(cubeTextureCoords[i]);
				}


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