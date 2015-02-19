#include <memory>
#include <iostream>
#include <ostream>

#include "Common.hpp"
#include "shader.hpp"
#include "RenderableComponent.hpp"

RenderableComponent::RenderableComponent() {
	//Generate the buffers
	//TODO: Only generate when needed
	glGenBuffers(1, &vboVertexId);
	glGenBuffers(1, &vboTextureId);
	glGenBuffers(1, &vboColourId);
	glGenBuffers(1, &vboIndexId);
} 
RenderableComponent::~RenderableComponent() {
	//Delete the buffers
	glDeleteBuffers(1, &vboVertexId);
	glDeleteBuffers(1, &vboTextureId);
	glDeleteBuffers(1, &vboColourId);
	glDeleteBuffers(1, &vboIndexId);
}
void RenderableComponent::setVertexData(std::vector<GLfloat> newVertexData, bool isDynamic) {;
	this->vertexData = newVertexData;

	setBufferData(vboVertexId, GL_ARRAY_BUFFER, this->vertexData, isDynamic);
}

void RenderableComponent::setTextureCoordsData(std::vector<GLfloat> newTextureData, bool isDynamic) {
	this->textureCoordsData = newTextureData;

	setBufferData(vboTextureId, GL_ARRAY_BUFFER, this->textureCoordsData, isDynamic);
}

void RenderableComponent::setColourData(std::vector<GLfloat>newColourData, bool isDynamic) {
	this->colourData = newColourData;

	setBufferData(vboColourId, GL_ARRAY_BUFFER, this->colourData, isDynamic);
}
void RenderableComponent::setIndexData(std::vector<GLfloat> newIndexData, bool isDynamic) {
	this->indexData = newIndexData;
	useIndexedVBO = true;

	setBufferData(vboIndexId, GL_ELEMENT_ARRAY_BUFFER, this->indexData, isDynamic);
}



void RenderableComponent::bindVBOs() {
	//Bind the vertex data buffer
	if (vertexData.size() > 0) {
		glEnableVertexAttribArray(0 /* VERTEX_POS_INDX */);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);
		glVertexAttribPointer(0 /*VERTEX_POS_INDX*/, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	//Bind the colour data buffer
	if (colourData.size() > 0) {
		glEnableVertexAttribArray(1 /* VERTEX_COLOUR_INDX */);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboColourId);
		glVertexAttribPointer(1 /*VERTEX_POS_INDX*/, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	//Bind the texture coordinate buffer
	if (textureCoordsData.size() > 0) {
		glEnableVertexAttribArray(2 /*VERTEX_TEXCOORD0_INDX*/);
		glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureId);
		glVertexAttribPointer(2 /* VERTEX_TEXCOORD0_INDX */, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	}
	if (useIndexedVBO) {
		//Bind the index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vboIndexId);
	}

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
	updateSubBuffer(vboVertexId, GL_ARRAY_BUFFER, offset, data);
}
void RenderableComponent::updateTextureBuffer(GLintptr offset, std::vector<GLfloat> data) {
	updateSubBuffer(vboTextureId, GL_ARRAY_BUFFER, offset, data);
}
void RenderableComponent::updateColourBuffer(GLintptr offset, std::vector<GLfloat> data) {
	updateSubBuffer(vboColourId, GL_ARRAY_BUFFER, offset, data);
}

void RenderableComponent::updateIndexBuffer(GLintptr offset, std::vector<GLfloat> data) {
	//TODO: Check set has been used first
	useIndexedVBO = true;

	updateSubBuffer(vboIndexId, GL_ELEMENT_ARRAY_BUFFER, offset, data);
}

void RenderableComponent::setBufferData(GLint bufferID, GLenum bufferType, std::vector<GLfloat> data, bool isDynamic) {

	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();

	//Set up buffer usage
	GLenum usage = GL_STATIC_DRAW;
	if (isDynamic)
		usage = GL_DYNAMIC_DRAW;

	glBindBuffer(bufferType, bufferID);

	if (data.size() > 0) {
		//Pass in data to the buffer buffer
		if (bufferType == GL_ELEMENT_ARRAY_BUFFER) {
			glBufferData(bufferType, data.size() * sizeof(GLuint), &data[0], usage);
		}
		else {
			glBufferData(bufferType, data.size() * sizeof(GLfloat), &data[0], usage);
		}

	}
	else {
		//Pass in data to the buffer buffer
		glBufferData(bufferType, 0, nullptr, usage);
	}

	//Release shader
	glUseProgram(id);
}

void RenderableComponent::updateSubBuffer(GLint bufferID, GLenum bufferType, GLintptr offset, std::vector<GLfloat> data) {
	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();
	glBindBuffer(bufferType, bufferID);

	//Update the buffer
	if (data.size() > 0) {
		if (bufferType == GL_ELEMENT_ARRAY_BUFFER) {
			glBufferSubData(bufferType, offset, data.size() * sizeof(GLuint), &data[0]);
		}
		else {
			glBufferSubData(bufferType, offset, data.size() * sizeof(GLfloat), &data[0]);
		}
	}
	else {
		glBufferSubData(bufferType, offset, 0, nullptr);
	}

	//Release shader
	glUseProgram(id);
}
