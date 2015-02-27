#include "Chunk.hpp"

#include "Common.hpp"
#include "Cube.hpp"
#include "LocationComponent.hpp"
#include "RenderableComponent.hpp"
#include "shader.hpp"
#include "SimplexNoise.hpp"
#include "TextureAtlas.hpp"

#include <iostream>
#include <memory>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <utility>

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
	std::shared_ptr<TextureAtlas> texture = std::make_shared<TextureAtlas>("resources/textures/texture_atlas.png");
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("resources//shaders//default_shader");
	if (!shader->isLoaded()) {
		std::cerr << "ERROR: In Chunk::init, cannot load shader." << std::endl;
		return;
	}

	renderableComponent->setShader(shader);
	renderableComponent->setTexture(texture);

	//Map of cube types (GRASS, WATER etc) to a string identifer
	std::map<int, std::string> cubeTypeToId;
	cubeTypeToId.insert(std::make_pair<int, std::string>(1, std::string("grass")));
	cubeTypeToId.insert(std::make_pair<int, std::string>(2, std::string("sand")));
	cubeTypeToId.insert(std::make_pair<int, std::string>(3, std::string("stone")));
	cubeTypeToId.insert(std::make_pair<int, std::string>(4, std::string("water")));

	//The number of cubes we actually want to render
	int numCubes = 0;

	//Dimensions
	int xLength = getChunkSizeX(), yLength = getChunkSizeY(), zLength = getChunkSizeZ();
	double xScale = 80.0;
	double zScale = 80.0;
	double yScale = 0.3;
	SimplexNoise noiseGenerator;

	//Generate the world 
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			//Generate noise 
			//[-1.0,1.0]
			double noise = (noiseGenerator.noise2D(x / xScale, z / zScale));

			//fill in y and below this block
			int yHeight = (int)(noise*(yLength/2) * yScale);
			for (int y = -yLength / 2; y < yHeight; ++y) {
				
				if (y < 4) {
					//Put sand here
					chunkData[x + (xLength / 2)][y + yLength / 2][z + (zLength / 2)] = 2;
				}
				else {
					chunkData[x + (xLength / 2)][y+yLength /2][z + (zLength / 2)] = 1;
				}
			}

			//clear above
			for (int y = yHeight; y < yLength; ++y) {
				chunkData[x + (xLength / 2)][y + yLength / 2][z + (zLength / 2)] = 0;
			}

			//Set data
			chunkNoiseData[x + (xLength / 2)][yHeight + yLength / 2][z + (zLength / 2)] = noise;
		}
	}

	//Put water on center
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			if (chunkData[x + (xLength / 2)][yLength/2][z + (zLength / 2)] == 0) {
				chunkData[x + (xLength / 2)][yLength/2][z + (zLength / 2)] = 4;
			}
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
	//	chunkData[2]; // top
	//	chunkData[3]; //bottom
	//	chunkData[4]; //front
	//	chunkData[5]; //back

	int numVertices = 0;
	//Generate renderable data[0]
	for (int x = -xLength / 2; x < xLength / 2; ++x) {
		for (int z = -zLength / 2; z < zLength / 2; ++z) {
			for (int y = -yLength / 2; y < yLength / 2; ++y) {

				int baseX = x + (xLength / 2);
				int baseY = y + (yLength / 2);
				int baseZ = z + (zLength / 2);

				//if not visible
				if (!chunkVisibleData[baseX][baseY][baseZ] || chunkData[baseX][baseY][baseZ] == 0)
					continue;
			
				float sf = 2.0f;
				float xPos = sf*(x);
				float yPos = sf*(y);
				float zPos = sf*(z);

				//Join faces together

				//generate face data

				//TODO improve efficiency of this entire class

				//top face
				//Do we need it?
				//We do if there is not a cube above
				if (baseY + 1 < yLength) {
					if (chunkData[baseX][baseY + 1][baseZ] == 0) {
						setupFaceData(baseX, baseY + 1, baseZ, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeTopFace, cubeTopTextureCoords);
					}
				}

				//bottom face
				//Do we need it?
				//We do if there is not a cube below
				if (baseY -1 > 0) {
					if (chunkData[baseX][baseY - 1][baseZ] == 0) {
						if (cubeTypeToId.at(chunkData[baseX][baseY][baseZ]) == "grass") {
							setupFaceData(baseX, baseY - 1, baseZ, xPos, yPos, zPos, &numVertices, "stone", texture, cubeBottomFace, cubeBottomTextureCoords);
						}
						else {
							setupFaceData(baseX, baseY - 1, baseZ, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeBottomFace, cubeBottomTextureCoords);
						}
					}
				}

				//front face
				//Do we need it?
				//We do if there is not a cube in front
				if (baseZ + 1 < zLength) {
					if (chunkData[baseX][baseY][baseZ + 1] == 0) {
						if (cubeTypeToId.at(chunkData[baseX][baseY][baseZ]) == "grass") {
							setupFaceData(baseX, baseY, baseZ + 1, xPos, yPos, zPos, &numVertices, "stone", texture, cubeFrontFace, cubeFrontTextureCoords);
						}
						else {
							setupFaceData(baseX, baseY, baseZ + 1, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeFrontFace, cubeFrontTextureCoords);
						}
						//setupFaceData(baseX, baseY, baseZ + 1, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeFrontFace, cubeFrontTextureCoords);
					}
				}

				//back face
				//Do we need it?
				//We do if there is not a cube behind
				if (baseZ - 1 > 0) {
					if (chunkData[baseX][baseY][baseZ - 1] == 0) {
						if (cubeTypeToId.at(chunkData[baseX][baseY][baseZ]) == "grass") {
							setupFaceData(baseX, baseY, baseZ - 1, xPos, yPos, zPos, &numVertices, "stone", texture, cubeBackFace, cubeBackTextureCoords);
						}
						else {
							setupFaceData(baseX, baseY, baseZ - 1, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeBackFace, cubeBackTextureCoords);
						}
						//setupFaceData(baseX, baseY, baseZ - 1, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeBackFace, cubeBackTextureCoords);
					}
				}

				//left face
				//Do we need it?
				//We do if there is not a cube to the left
				if (baseX - 1 > 0) {
					if (chunkData[baseX - 1][baseY][baseZ] == 0) {
						if (cubeTypeToId.at(chunkData[baseX][baseY][baseZ]) == "grass") {
							setupFaceData(baseX - 1, baseY, baseZ, xPos, yPos, zPos, &numVertices, "stone", texture, cubeLeftFace, cubeLeftTextureCoords);
						}
						else {
							setupFaceData(baseX - 1, baseY, baseZ, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeLeftFace, cubeLeftTextureCoords);
						}
						//setupFaceData(baseX - 1, baseY, baseZ, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeLeftFace, cubeLeftTextureCoords);
					}
				}

				//right face 
				//Do we need it?
				//We do if there is not a cube to the right
				if (baseX + 1 < xLength) {
					if (chunkData[baseX+1][baseY][baseZ] == 0) {
						if (cubeTypeToId.at(chunkData[baseX][baseY][baseZ]) == "grass") {
							setupFaceData(baseX + 1, baseY, baseZ, xPos, yPos, zPos, &numVertices, "stone", texture, cubeRightFace, cubeRightTextureCoords);
						}
						else {
							setupFaceData(baseX + 1, baseY, baseZ, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeRightFace, cubeRightTextureCoords);
						}
						//setupFaceData(baseX + 1, baseY, baseZ, xPos, yPos, zPos, &numVertices, cubeTypeToId.at(chunkData[baseX][baseY][baseZ]), texture, cubeRightFace, cubeRightTextureCoords);
					}
				}
				/*	for (size_t i = 0; i < sizeof(cubeColours) / sizeof(GLfloat); ++i) {
					chunkColourData.push_back(cubeColours[i]);
				}
				*/
			}
		}
	}


	//set renderable data
	renderableComponent->setVertexData(chunkVertexData, false);
	renderableComponent->setNumVerticesRender(numVertices);
	renderableComponent->setColourData(chunkColourData, false);
	renderableComponent->setTextureCoordsData(chunkTextureCoordsData, false);

}

void Chunk::setupFaceData(int cubeX, int cubeY, int cubeZ, float xPos, float yPos, float zPos, int *numVertices,
	std::string tileName, std::shared_ptr<TextureAtlas> texture, const glm::vec3 (&vertexData)[6],
	const GLfloat (&texData)[6*2]) {
	for (size_t i = 0; i < sizeof(vertexData) / sizeof(glm::vec3); ++i) {
		//div by 2 as cube data is -1 to +1
		glm::vec3 newCubeData = vertexData[i] * 1.0f;
		newCubeData.x += xPos;
		newCubeData.y += yPos;
		newCubeData.z += zPos;
		chunkVertexData.push_back(newCubeData.x);
		chunkVertexData.push_back(newCubeData.y);
		chunkVertexData.push_back(newCubeData.z);
		(*numVertices)++;
	}


	std::tuple<float, float, float, float> rect = texture->calculateTextureCoords(tileName);
	//pairs of (x, y)
	//(0.0, 0.0) -> (x,y)
	//(1.0, 0.0) -> (x + width,y)
	//(0.0, 1.0) -> (x, y + height)
	//(1.0, 1.0) -> (x + width, y + height)
	float xOffset = std::get<0>(rect);
	float yOffset = std::get<1>(rect);
	float width = std::get<2>(rect);
	float height = std::get<3>(rect);
	for (size_t i = 0; i < sizeof(texData) / sizeof(GLfloat); i += 2) {
		if (texData[i] == 0.0f && texData[i + 1] == 0.0f) {
			chunkTextureCoordsData.push_back(xOffset);
			chunkTextureCoordsData.push_back(yOffset);
		}
		else if (texData[i] == 1.0f && texData[i + 1] == 0.0f) {
			chunkTextureCoordsData.push_back(xOffset + width);
			chunkTextureCoordsData.push_back(yOffset);
		}
		else if (texData[i] == 0.0f && texData[i + 1] == 1.0f) {
			chunkTextureCoordsData.push_back(xOffset);
			chunkTextureCoordsData.push_back(yOffset + height);
		}
		else if (texData[i] == 1.0f && texData[i + 1] == 1.0f) {
			chunkTextureCoordsData.push_back(xOffset + width);
			chunkTextureCoordsData.push_back(yOffset + height);
		}
		else {
			assert(false);
			std::cerr << "ERROR: In Chunk, texture coords calc. Comparisions failed.";
		}
	}
}

const glm::vec3 Chunk::cubeTopFace[6] {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),

	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f)
};

const glm::vec3 Chunk::cubeBottomFace[6]{
	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),

	glm::vec3(1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f)
};

const glm::vec3 Chunk::cubeLeftFace[6] {
	glm::vec3(-1.0f, -1.0f, -1.0f), // triangle 1 : begin
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f), // triangle 1 : end

	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f)
};

const glm::vec3 Chunk::cubeRightFace[6] {
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, -1.0f),

	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f)
};

const glm::vec3 Chunk::cubeFrontFace[6]{
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, -1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f),

	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(-1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, -1.0f, 1.0f)
};

const glm::vec3 Chunk::cubeBackFace[6]{
	glm::vec3(1.0f, 1.0f, -1.0f), // triangle 2 : begin
	glm::vec3(-1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, 1.0f, -1.0f), // triangle 2 : end

	glm::vec3(1.0f, 1.0f, -1.0f),
	glm::vec3(1.0f, -1.0f, -1.0f),
	glm::vec3(-1.0f, -1.0f, -1.0f),

};

const GLfloat Chunk::cubeFrontTextureCoords[6 * 2] {
	0.0f, 0.0,
	0.0f, 1.0f,
	1.0f, 1.0f,

	1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
};

const GLfloat Chunk::cubeBackTextureCoords[6 * 2] {
	0.0, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
};

const GLfloat Chunk::cubeLeftTextureCoords[6 * 2] {
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,

	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};

const GLfloat Chunk::cubeRightTextureCoords[6 * 2] {
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
};

const GLfloat Chunk::cubeTopTextureCoords[6 * 2]{
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,

	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};

const GLfloat Chunk::cubeBottomTextureCoords[6 * 2] {
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,

	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0, 0.0f,
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