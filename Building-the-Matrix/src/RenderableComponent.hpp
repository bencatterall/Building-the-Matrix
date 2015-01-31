#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H
#include "Common.hpp"
#include "shader.hpp"
#include <memory>


///
/// This class holds all the referrences to the data needed to render
/// the geometry. The idea is that this data is generated and then put
/// into this object so that it then owns this data and is responsible for
/// cleaning it up. Then, when an object is rendered, this object's
/// information is queried and used in the rendering loops.
///
/// For example, each object has a RenderableComponent and then in the
/// MapViewer render loop, each one is fetched and then used to draw the object
///
/// The aim is to separate the rendering code from the object and Map
/// classes and the logic in these classes so that we reduce code
/// bloat and needless copy-pasting.
///
/// THIS CLASS DELETES THE MEMORY IT IS GIVEN WHEN IT IS DESTRUCTED
///
/// Rework of code written by Ben for the pyland project.
///
class RenderableComponent {
	///
	/// The buffer holding the vertex data
	///
	GLfloat* vertexData = nullptr;
	///
	/// The size of the vertex data in bytes
	///
	size_t vertexDataSize = 0;
	///
	/// The number of vertices to render
	///
	GLsizei numVerticesRender = 0;
	///
	/// The buffer holding the texture coordinate data
	///
	GLfloat* textureCoordsData = nullptr;
	///
	/// The size of the texture coordinate buffer in bytes
	///
	size_t textureCoordsDataSize = 0;
	///
	/// The vertex buffer object identifier for the vertex buffer
	///
	GLuint vboVertexId = 0;
	///
	/// The vertex buffer object identifier for the texture buffer
	///
	GLuint vboTextureId = 0;

	///
	/// The shader used to render this component. This is a pointer because more than
	/// one RenderableComponent can have the same shader
	///
	std::shared_ptr<Shader> shader;

	///
	/// The current projection matrix
	///
	glm::mat4 projectionMatrix = glm::mat4(1.0);

	///
	/// the current modelview matrix
	///
	glm::mat4 modelviewMatrix = glm::mat4(1.0);

public:
	RenderableComponent();
	~RenderableComponent();
	///
	/// Get the projection matrix
	/// @return the projection matrix
	///
	glm::mat4 getProjectionMatrix() { return projectionMatrix; }

	///
	/// Set the projection matrix for this component
	/// @new_projection_matrix the new matrix
	///
	void setProjectionMatrix(glm::mat4 newProjectionMatrix) { projectionMatrix = newProjectionMatrix; }

	///
	/// Get the modelview matrix
	/// @return the modelview matrix for this component
	///
	glm::mat4 getModelviewMatrix() { return modelviewMatrix; }

	///
	/// Set the modelview matrix for this component
	/// @param new_modelview_matrix the new modelview matrix
	///
	void setModelviewMatrix(glm::mat4 newModelviewMatrix) { modelviewMatrix = newModelviewMatrix; }

	///
	/// Bind the shader program to the Opengl pipeline to use it for rendering
	///
	void bindShader();

	///
	/// Release the shader program from the Opengl pipeline
	///
	void releaseShader();

	///
	/// Sets the shader to use for this component
	///
	void setShader(std::shared_ptr<Shader> newShader) { shader = newShader;}

	///
	/// Gets the shader used by this component
	///
	std::shared_ptr<Shader> getShader() { return shader; }

	///
	/// Bind the vertex buffers
	///
	void bindVBOs();

	///
	/// Release the vertex buffers
	///
	void releaseVBOs();

	///
	/// Get a pointer to the vertex data
	///
	GLfloat* getVertexData() { return vertexData; }

	///
	/// Get the size of the vertex data
	///
	size_t getVertexDataSize() { return vertexDataSize; }

	///
	/// Set the vertex data to use for this component.
	/// @param new_vertex_data The new data to use for ther vertices of this object
	/// @param the size of the data in GLfloats
	/// @param is_dynamic If true, then the data for this buffer will be changed often. If false, it is static geometry
	///
	void setVertexData(GLfloat* newVertexData, size_t dataSize, bool isDynamic);

	///
	/// Get a pointer to the texture coordinate data
	///
	GLfloat* getTextureCoordsData() { return textureCoordsData; }

	///
	/// Set the texture coordinate data to use for this component
	/// @param new_texture_data The new data to use for ther tehadxture coordinates of this object
	/// @param data_size The size of the data in GLfloats
	/// @param is_dynamic If true, then the data for this buffer will be changed often. If false, it is static
	///
	void setTextureCoordsData(GLfloat* newTextureData, size_t dataSize, bool isDynamic);

	///
	/// Get the texture coords data size
	///
	size_t getTextureCoordsDataSize() { return textureCoordsDataSize; }

	///
	/// Bind the texture objects
	///
	void bindTextures();

	///
	/// Release the texture objects
	///
	void releaseTextures();
	///
	/// Get the number of vertices to render
	///
	GLsizei getNumVerticesRender() { return numVerticesRender; }

	///
	/// Set the number of vertices we need to render
	///
	void setNumVerticesRender(GLsizei numVertices) { numVerticesRender = numVertices; }

	///
	/// Update the texture buffer
	/// @param offset the byte offset into the buffer
	/// @param size the size of the data to put into the buffer in bytes
	/// @param data the data to put into the buffer
	///
	void updateTextureBuffer(GLintptr offset, size_t size, GLfloat* data);

	///
	/// Update the vertex buffer
	/// @param offset the byte offset into the buffer
	/// @param size the size of the data to put into the buffer in bytes
	/// @param data the data to put into the buffer
	///
	void updateVertexBuffer(GLintptr offset, size_t size, GLfloat* data);
};
#endif