#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H
#include "Common.hpp"
#include "shader.hpp"
#include "Texture.hpp"
#include <memory>
#include <vector>

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
	/// The texture for this object
	///
	std::shared_ptr<Texture> texture;

	///
	/// The buffer holding the vertex data
	///
	std::vector<GLfloat> vertexData;

	///
	/// The indices buffer
	///
	std::vector<GLfloat> indexData;

	///
	/// The number of vertices to render
	///
	GLsizei numVerticesRender = 0;

	///
	/// The buffer holding the texture coordinate data
	///
	std::vector<GLfloat> textureCoordsData ;

	///
	/// The buffer holding the colour coordinate data
	///
	std::vector<GLfloat> colourData;

	///
	/// The vertex buffer object identifier for the vertex buffer
	///
	GLuint vboVertexId = 0;

	///
	/// The vertex buffer object identifier for the texture buffer
	///
	GLuint vboTextureId = 0;

	///
	/// The colour buffer object identifier for the colour buffer
	///
	GLuint vboColourId = 0;

	///
	/// The index buffer object identifier for the index buffer
	///
	GLuint vboIndexId = 0;


	bool useIndexedVBO = false;


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


	void setBufferData(GLint bufferID, GLenum bufferType, std::vector<GLfloat> data, bool isDynamic);
	void updateSubBuffer(GLint bufferID, GLenum bufferType, GLintptr offset, std::vector<GLfloat> data);

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

	void setTexture(std::shared_ptr<Texture> texture) { this->texture = texture; }

	std::shared_ptr<Texture> getTexture() { return texture; }

	bool usesIndexedVBO() { return useIndexedVBO; }

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
	std::vector<GLfloat> getVertexData() { return vertexData; }

	///
	/// Get the size of the vertex data
	///
	size_t getVertexDataSize() { return vertexData.size(); }

	///
	/// Set the vertex data to use for this component.
	/// @param new_vertex_data The new data to use for ther vertices of this object
	/// @param is_dynamic If true, then the data for this buffer will be changed often. If false, it is static geometry
	///
	void setVertexData(std::vector<GLfloat> newVertexData, bool isDynamic);

	///
	/// Get a pointer to the index data
	///
	std::vector<GLfloat> getIndexData() { return indexData; }

	///
	/// Get the size of the vertex data
	///
	size_t getIndexDataSize() { return indexData.size(); }

	///
	/// Set the index data to use for this component.
	/// @param new_index_data The new data to use for ther indices of this object
	/// @param is_dynamic If true, then the data for this buffer will be changed often. If false, it is static geometry
	///
	void setIndexData(std::vector<GLfloat> newIndexData, bool isDynamic);

	///
	/// Get a pointer to the colour data
	///
	std::vector<GLfloat> getColourData() { return colourData; }

	///
	/// Get the size of the colour data
	///
	size_t getColourDataSize() { return colourData.size(); }

	///
	/// Set the colour data to use for this component.
	/// @param newColourData The new data to use for the per-vertex colours of this object
	/// @param is_dynamic If true, then the data for this buffer will be changed often. If false, it is static geometry
	///
	void setColourData(std::vector<GLfloat> newColourData, bool isDynamic);


	///
	/// Get a vector of the texture coordinate data
	///
	std::vector<GLfloat> getTextureCoordsData() { return textureCoordsData; }

	///
	/// Set the texture coordinate data to use for this component
	/// @param new_texture_data The new data to use for ther tehadxture coordinates of this object
	/// @param is_dynamic If true, then the data for this buffer will be changed often. If false, it is static
	///
	void setTextureCoordsData(std::vector<GLfloat> newTextureData, bool isDynamic);

	///
	/// Get the texture coords data size
	///
	size_t getTextureCoordsDataSize() { return textureCoordsData.size(); }

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
	void updateTextureBuffer(GLintptr offset, std::vector<GLfloat> data);

	///
	/// Update the vertex buffer
	/// @param offset the byte offset into the buffer
	/// @param size the size of the data to put into the buffer in bytes
	/// @param data the data to put into the buffer
	///
	void updateVertexBuffer(GLintptr offset, std::vector<GLfloat> data);

	///
	/// Update the colour buffer
	/// @param offset the byte offset into the buffer
	/// @param size the size of the data to put into the buffer in bytes
	/// @param data the data to put into the buffer
	///
	void updateColourBuffer(GLintptr offset, std::vector<GLfloat> data);

	///
	/// Update the indices buffer
	/// @param offset the byte offset into the buffer
	/// @param size the size of the data to put into the buffer in bytes
	/// @param data the data to put into the buffer
	///
	void updateIndexBuffer(GLintptr offset, std::vector<GLfloat> data);
};
#endif