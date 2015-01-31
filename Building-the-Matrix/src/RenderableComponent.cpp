#include <memory>
#include <ostream>

#include "shader.hpp"
#include "RenderableComponent.hpp"
#define VERTEX_POS_INDX 0
#define VERTEX_TEXCOORD0_INDX 1
RenderableComponent::RenderableComponent() {
	//Generate the vertex buffers
	glGenBuffers(1, &vboVertexId);
	glGenBuffers(1, &vboTextureId);
} 
RenderableComponent::~RenderableComponent() {
	//Delete the vertex buffers
	glDeleteBuffers(1, &vboVertexId);
	glDeleteBuffers(1, &vboTextureId);
	delete[] vertexData;
	delete[] textureCoordsData;
}
void RenderableComponent::setVertexData(GLfloat* newVertexData, size_t dataSize, bool isDynamic) {
	delete[] vertexData;
	this->vertexData = newVertexData;
	this->vertexDataSize = dataSize;

	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();

	//Set up buffer usage
	GLenum usage = GL_STATIC_DRAW;
	if (isDynamic)
		usage = GL_DYNAMIC_DRAW;

	//Pass in data to the buffer buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);
	glBufferData(GL_ARRAY_BUFFER, this->vertexDataSize, this->vertexData, usage);

	//Restore previous program
	glUseProgram(id);
}

void RenderableComponent::setTextureCoordsData(GLfloat* newTextureData, size_t dataSize, bool isDynamic) {
	delete[] textureCoordsData;
	this->textureCoordsData = newTextureData;
	this->textureCoordsDataSize = dataSize;

	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();

	//Set up buffer usage
	GLenum usage = GL_STATIC_DRAW;
	if (isDynamic)
		usage = GL_DYNAMIC_DRAW;

	//Pass in data to the buffer buffer
	glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureId);
	glBufferData(GL_ARRAY_BUFFER, this->textureCoordsDataSize, this->textureCoordsData, usage);

	//Release shader
	glUseProgram(id);
}
void RenderableComponent::bindVBOs() {
	//Bind the vertex data buffer
	glEnableVertexAttribArray(0 /* VERTEX_POS_INDX */);
	glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);
	glVertexAttribPointer(0 /*VERTEX_POS_INDX*/, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	
	//glEnableVertexAttribArray(1 /*VERTEX_TEXCOORD0_INDX*/);
	//glBindBuffer(GL_ARRAY_BUFFER, this->vboTextureId);
	//glVertexAttribPointer(1 /* VERTEX_TEXCOORD0_INDX */, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


	//set sampler texture to unit 0
	//TODO: Check this is ok
	//glUniform1i(glGetUniformLocation(shader->getProgram(), "s_texture"), 0);
}
void RenderableComponent::bindTextures() {
	glActiveTexture(GL_TEXTURE0);
	//Bind tiles texture
	//TODO: FINISH BINDING
	//glBindTexture(GL_TEXTURE_2D, texture_atlas->get_gl_texture());
}
void RenderableComponent::releaseTextures() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
void RenderableComponent::releaseVBOs() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void RenderableComponent::bindShader() {
	if (!shader){
		std::cout << "ERROR" << std::endl;
		//exit(0);
	}// exit(0); }
		

	glUseProgram(shader->getProgram());
}
void RenderableComponent::releaseShader() {
	glUseProgram(0);
}
void RenderableComponent::updateVertexBuffer(GLintptr offset, size_t size, GLfloat* data) {
	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();
	glBindBuffer(GL_ARRAY_BUFFER, this->vboVertexId);

	//Update the buffer
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

	//Release shader
	glUseProgram(id);
}
void RenderableComponent::updateTextureBuffer(GLintptr offset, size_t size, GLfloat* data) {
	//Get current shader
	GLint id;
	glGetIntegerv(GL_CURRENT_PROGRAM, &id);

	//Bind our shader
	bindShader();
	glBindBuffer(GL_ARRAY_BUFFER, vboTextureId);

	//Update the buffer
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);

	//Release shader
	glUseProgram(id);
}