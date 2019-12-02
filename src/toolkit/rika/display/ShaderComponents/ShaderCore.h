/**
 * Provides some common thing that are repeatedly defined in all the shaders. (Creating program, linking it, checking errors, etc.)
 *
 * @date: 4/5/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef SHADER_CORE_H
#define SHADER_CORE_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../TextureImage.h"
#include "ContainerVBO.h"
#include "../Camera.h"


namespace universalShaUni {

	// uniform locations for most shaders

	const int mvp = 0;
	const int screensize = 1;
}

class ShaderCore {
private:
	char* pathVertShader;
	char* pathGeomShader;
	char* pathFragShader;
	GLuint shaderProgram;
	GLuint vertexShader, geometryShader, fragmentShader;
public:
	void setVertexShader(char* path);
	void setGeometryShader(char* path);
	void setFragmentShader(char* path);
	bool buildProgram();
	void bindProgram();
	void deleteProgram();
	GLuint getProgramId();
	ShaderCore();
	~ShaderCore();
};

#endif