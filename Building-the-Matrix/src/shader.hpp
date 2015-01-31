#ifndef SHADER_H
#define SHADER_H
#include "Common.hpp"
#include <string>
#include <fstream>
#include <sstream>

///
/// An Opengl shader program loader, compiler and manager.
///
class Shader {
	///
	/// The name of the GLSL shader program file
	///
	std::string program_name;

	///
	/// The OpenGL program object identifier
	///
	GLuint programObj;
	GLuint fragmentShader;
	GLuint vertexShader;
	bool loaded = false;

	GLuint loadShader(GLenum type, const std::string src);
	std::string Shader::loadFile(std::string fileName);
public:
	Shader(const std::string program_name);
	Shader(const std::string vs, const std::string fs);
	~Shader();

	///
	/// Thrown to indicate a shader loading error
	///
	class LoadException : public std::runtime_error {

	public:
		LoadException(const char* message) : std::runtime_error(message) {};
		LoadException(const std::string &message) :std::runtime_error(message) {};
	};

	bool isLoaded() { return loaded;  }

	///
	/// Get  the OpenGL shader program object ID
	/// @return the OpenGL shader program object ID
	///
	GLuint getProgram() { return programObj; }

};
#endif