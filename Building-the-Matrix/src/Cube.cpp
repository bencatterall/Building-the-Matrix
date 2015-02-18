#include "Cube.hpp"

Cube::Cube() {

	//TODO improve efficiency of this entire class
	for (size_t i = 0; i < sizeof(cubeData) / sizeof(GLfloat); ++i) {
		cubeVertexData.push_back(4.0f*cubeData[i]);
	}

	for (size_t i = 0; i < sizeof(cubeColours) / sizeof(GLfloat); ++i) {
		cubeColourData.push_back(cubeColours[i]);
	}

	for (size_t i = 0; i < sizeof(cubeTextureCoords) / sizeof(GLfloat); ++i) {
		cubeTextureCoordsData.push_back(cubeTextureCoords[i]);
	}

}

const GLfloat Cube::cubeData[3*36] = {
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

const GLfloat Cube::cubeColours[4*36] = {
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

const GLfloat Cube::cubeTextureCoords[2 * 36] = {
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
};/* 		0.0f, 1.0f,// triangle 1 : begin
	1.0f, 1.0f,
	1.0f, 0.0f, // triangle 1 : end

	0.0f, 0.0f, // triangle 2 : begin
	1.0f, 1.0f,
	0.0f, 1.0f, // triangle 2 : end

	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	0.5f, 1.0f,
	1.0f, 1.0f,

	0.0f, 0.0f,
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
	1.0f, 1.0f,*/