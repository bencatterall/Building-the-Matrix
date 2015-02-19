#include "Cube.hpp"
#include "LocationComponent.hpp"
#include "RenderableComponent.hpp"
#include "Physics\PhysicsObject.hpp"
#include "shader.hpp"	

Cube::Cube(glm::vec3 pos) {

	//TODO improve efficiency of this entire class
	for (size_t i = 0; i < sizeof(cubeData) / sizeof(GLfloat); ++i) {
		cubeVertexData.push_back(10.0f*cubeData[i]);
	}

	for (size_t i = 0; i < sizeof(cubeColours) / sizeof(GLfloat); ++i) {
		cubeColourData.push_back(cubeColours[i]);
	}

	for (size_t i = 0; i < sizeof(cubeTextureCoords) / sizeof(GLfloat); ++i) {
		cubeTextureCoordsData.push_back(cubeTextureCoords[i]);
	
	
	}

	std::shared_ptr<RenderableComponent> renderableComponent = this->getRenderableComponent();
	std::shared_ptr<Shader> shader = std::make_shared<Shader>("resources//shaders//default_shader");
				
	std::shared_ptr<LocationComponent> locationComponent = this->getLocationComponent();
				
	locationComponent->setPosition(pos);
				
	if (!shader->isLoaded())
		exit(0);
				
	renderableComponent->setShader(shader);
	renderableComponent->setVertexData(cubeVertexData, false);
	renderableComponent->setNumVerticesRender(36);
	renderableComponent->setColourData(cubeColourData, false);
	physicsObject = std::make_shared<PhysicsObject>(locationComponent, renderableComponent);
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