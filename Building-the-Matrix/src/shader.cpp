#include <string>
#include "shader.hpp"


Shader::Shader(const std::string program_name) :
	Shader(program_name + ".vs", program_name + ".fs")
{

}

Shader::Shader(const std::string vs, const std::string fs) 
{
	GLint linked = 0;

	//Load fragment and vertex shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, loadFile(vs).c_str());
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, loadFile(fs).c_str());

	//Create the program
	programObj = glCreateProgram();

	//Program object
	if (!programObj) {
#ifdef DEBUG
		std::cerr << "ERROR: Shader creation, couldn't create program object. " << std::endl;
#endif
		throw Shader::LoadException("Unable to create program shader");
	}

	//Attach the shader objects
	glAttachShader(programObj, vertexShader);
	glAttachShader(programObj, fragmentShader);

	// Temporary hack before restructuring.
	// TODO: Untemparary-ify this.
	glBindAttribLocation(programObj, /* VERTEX_POS_INDX */ 0, "a_position");
	//glBindAttribLocation(programObj, /* VERTEX_TEXCOORD0_INDX */ 1, "a_texCoord");

	//Link it
	glLinkProgram(programObj);

	//Check linking status
	glGetProgramiv(programObj, GL_LINK_STATUS, &linked);

	//Handle linking failure
	if (!linked) {
#ifdef DEBUG
		GLint infoLen = 0;
		glGetProgramiv(programObj, GL_INFO_LOG_LENGTH, &infoLen);
		
		if (infoLen > 1) {
			char* infoLog = new char[sizeof(char)*infoLen];

			glGetProgramInfoLog(programObj, infoLen, nullptr, infoLog);
			std::cerr << "ERROR: In linking program " << infoLog << std::endl;
			delete[] infoLog;
		}
#endif

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteProgram(programObj);
		throw Shader::LoadException("Failed to link shader program.");
	}

	//Shader program can now be used
	loaded = true;
}

Shader::~Shader() {
	//Cleanup resources
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(programObj);
}


std::string Shader::loadFile(std::string fileName) {
	std::ifstream file(fileName);
	std::stringstream output;

	output << file.rdbuf();
	return output.str();
}


GLuint Shader::loadShader(GLenum type, const std::string src) {
	GLuint shader;
	GLint compiled = 0;

	shader = glCreateShader(type);

	if (!shader) {
		//Could't create shader
#ifndef DEBUG
		cerr << "ERROR: Failed to create shader: " << src << std::endl;
#endif
		return 0;
	}

	//Load shader source code
	const char * source = src.c_str();
	glShaderSource(shader, 1, &source, nullptr);

	//Compile the shader
	glCompileShader(shader);

	//Check for failure
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	//Handle compilation failure
	if (!compiled) {
#ifdef DEBUG
		//Get debug info
		GLint infoLen = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1){
			char* infoLog = new char[sizeof(char)*infoLen];

			glGetShaderInfoLog(shader, infoLen, nullptr, infoLog);
			std::cerr << "ERROR: In compiling shader " << infoLog << std::endl;
			delete[] infoLog;
		}
#endif
		//cleanup
		glDeleteShader(shader);
		throw Shader::LoadException("Unable to create shader");
	}

	return shader;
}