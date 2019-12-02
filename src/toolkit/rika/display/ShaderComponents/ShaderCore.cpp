/**
 * Provides some common thing that are repeatedly defined in all the shaders. (Creating program, linking it, checking errors, etc.)
 *
 * @date: 4/5/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "ShaderCore.h"

bool readShaderText(const std::string& filename, std::string& out) {
	std::ifstream file;
	file.open(filename);
	if (!file) {
		return false;
	}
	std::stringstream stream;
	stream << file.rdbuf();
	file.close();
	out = stream.str();
	return true;
}

bool loadShader(GLuint type, const char* path, GLuint* shaderTgt) {
	std::string shData;
	readShaderText(path, shData);
	const char *shDatacstr = shData.c_str();
	*shaderTgt = glCreateShader(type);
	glShaderSource(*shaderTgt, 1, &shDatacstr, NULL);
	glCompileShader(*shaderTgt);
	GLint success = 0;
	glGetShaderiv(*shaderTgt, GL_COMPILE_STATUS, &success);
	if (success != GL_TRUE) {
		GLint maxLength = 0;
		glGetShaderiv(*shaderTgt, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(*shaderTgt, maxLength, &maxLength, &errorLog[0]);
		glDeleteShader(*shaderTgt);
		fprintf(stderr, "Error: Shader Compile Error: '%s'\n%s\n", path, &errorLog[0]);
		return true;
	}
	return false;
}

bool ShaderCore::buildProgram() {
	if (loadShader(GL_VERTEX_SHADER, pathVertShader, &vertexShader)) {
		return true;
	}

	//skip geometry shader if not provided.
	if(pathGeomShader != NULL)
		if (loadShader(GL_GEOMETRY_SHADER, pathGeomShader, &geometryShader)) {
			glDeleteShader(vertexShader);
			return true;
		}

	if (loadShader(GL_FRAGMENT_SHADER, pathFragShader, &fragmentShader)) {
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		return true;
	}

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, geometryShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	GLint isLinked = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(vertexShader);
		glDeleteShader(geometryShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(shaderProgram);
		fprintf(stderr, "Error: Shader link Error.\n%s\n", &infoLog[0]);
		return true;
	}
	//
	return false;
}

void ShaderCore::bindProgram() {
	glUseProgram(shaderProgram);
}

void ShaderCore::setVertexShader(char* path) {
	pathVertShader = path;
}

void ShaderCore::setGeometryShader(char* path) {
	pathGeomShader = path;
}

void ShaderCore::setFragmentShader(char* path) {
	pathFragShader = path;
}

void ShaderCore::deleteProgram() {
	glDeleteShader(vertexShader);
	glDeleteShader(geometryShader);
	glDeleteShader(fragmentShader);
	glDeleteProgram(shaderProgram);
}

GLuint ShaderCore::getProgramId() {
	return shaderProgram;
}

ShaderCore::ShaderCore() {

}

ShaderCore::~ShaderCore() {

}