#include <memory>
#include <ostream>

#include "shader.hpp"
#include "RenderableComponent.hpp"

RenderableComponent::RenderableComponent() {
	//Generate the vertex buffers
	glGenBuffers(1, &vboVertexId);
	glGenBuffers(1, &vboTextureId);
	glGenBuffers(1, &vboColourId);
} 
RenderableComponent::~RenderableComponent() {
	//Delete the vertex buffers
	glDeleteBuffers(1, &vboVertexId);
	glDeleteBuffers(1, &vboTextureId);
	glDeleteBuffers(1, &vboColourId);
}
void RenderableComponent::setVertexData(std::vector<GLfloat> newVertexData, bool isDynamic) {;
	this->vertexData = newVertexData;

	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();

	//Set up buffer usage
	GLenum usage = GL_STATIC_DRAW;
	if (isDynamic)
		usage = GL_DYNAMIC_DRAW;

	if (vertexData.size() > 0) {
		//Pass in data to the buffer buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);
		glBufferData(GL_ARRAY_BUFFER, this->vertexData.size()  * sizeof(GLfloat), &this->vertexData[0], usage);
	}
	else {
		//Pass in data to the buffer buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, usage);
	}
	//Restore previous program
	glUseProgram(id);
}

void RenderableComponent::setTextureCoordsData(std::vector<GLfloat> newTextureData, bool isDynamic) {
	this->textureCoordsData = newTextureData;

	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();

	//Set up buffer usage
	GLenum usage = GL_STATIC_DRAW;
	if (isDynamic)
		usage = GL_DYNAMIC_DRAW;

	if (textureCoordsData.size() > 0) {
		//Pass in data to the buffer buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureId);
		glBufferData(GL_ARRAY_BUFFER, this->textureCoordsData.size() * sizeof(GLfloat), &this->textureCoordsData[0], usage);
	}
	else {
		//Pass in data to the buffer buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureId);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, usage);
	}
	//Release shader
	glUseProgram(id);
}

void RenderableComponent::setColourData(std::vector<GLfloat>newColourData, bool isDynamic) {
	this->colourData = newColourData;

	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();

	//Set up buffer usage
	GLenum usage = GL_STATIC_DRAW;
	if (isDynamic)
		usage = GL_DYNAMIC_DRAW;

	if (colourData.size() > 0) {
		//Pass in data to the buffer buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboColourId);
		glBufferData(GL_ARRAY_BUFFER, this->colourData.size() * sizeof(GLfloat), &this->colourData[0], usage);
	}
	else {
		//Pass in data to the buffer buffer
		glBindBuffer(GL_ARRAY_BUFFER, this->vboColourId);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, usage);
	}

	//Release shader
	glUseProgram(id);
}
void RenderableComponent::bindVBOs() {
	//Bind the vertex data buffer
	glEnableVertexAttribArray(0 /* VERTEX_POS_INDX */);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);
	glVertexAttribPointer(0 /*VERTEX_POS_INDX*/, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	//Bind the colour data buffer
	glEnableVertexAttribArray(1 /* VERTEX_COLOUR_INDX */);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboColourId);
	glVertexAttribPointer(1 /*VERTEX_POS_INDX*/, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
		
	glEnableVertexAttribArray(2 /*VERTEX_TEXCOORD0_INDX*/);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureId);
	glVertexAttribPointer(2 /* VERTEX_TEXCOORD0_INDX */, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

	//set sampler texture to unit 0
	//TODO: Check this is ok
	glUniform1i(glGetUniformLocation(shader->getProgram(), "s_texture"), 0);
}
void RenderableComponent::bindTextures() {
	glActiveTexture(GL_TEXTURE0);
	//Bind tiles texture
	//TODO: FINISH BINDING
	if (!texture)
		return;
	glBindTexture(GL_TEXTURE_2D, texture->getID());
}
void RenderableComponent::releaseTextures() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
void RenderableComponent::releaseVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void RenderableComponent::bindShader() {
	if (!shader){
		std::cout << "ERROR: Can't bind shader" << std::endl;
		return;
	}
		
	glUseProgram(shader->getProgram());
}
void RenderableComponent::releaseShader() {
	glUseProgram(0);
}
void RenderableComponent::updateVertexBuffer(GLintptr offset, std::vector<GLfloat> data) {
	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();
	glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);

	//Update the buffer
	if (data.size() > 0) {
		glBufferSubData(GL_ARRAY_BUFFER, offset, data.size(), &data[0]);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, offset, 0, nullptr);
	}
	//Release shader
	glUseProgram(id);
}
void RenderableComponent::updateTextureBuffer(GLintptr offset, std::vector<GLfloat> data) {
	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();
	glBindBuffer(GL_ARRAY_BUFFER, vboTextureId);

	//Update the buffer
	if (data.size() > 0) {
		glBufferSubData(GL_ARRAY_BUFFER, offset, data.size(), &data[0]);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, offset, 0, nullptr);
	}
	//Release shader
	glUseProgram(id);
}
void RenderableComponent::updateColourBuffer(GLintptr offset, std::vector<GLfloat> data) {
	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();
	glBindBuffer(GL_ARRAY_BUFFER, vboColourId);

	//Update the buffer
	//Update the buffer
	if (data.size() > 0) {
		glBufferSubData(GL_ARRAY_BUFFER, offset, data.size(), &data[0]);
	}
	else {
		glBufferSubData(GL_ARRAY_BUFFER, offset, 0, nullptr);
	}

	//Release shader
	glUseProgram(id);
}