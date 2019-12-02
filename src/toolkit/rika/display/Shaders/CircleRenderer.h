/**
 * Handles the shader for rendering circles.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef CIRCLE_RENDERER_H
#define CIRCLE_RENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string.h>
#include <sstream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../TextureImage.h"
#include "../ShaderComponents/ContainerVBO.h"
#include "../ShaderComponents/ShaderCore.h"
#include "../UI/UISprite.h"

namespace circleShaAtt {

	// attribute locations for the Circle Renderer

	const int posrad = 0; //posx posy, r1 - outer, r2 - inner
	const int highlight = 1; //vec4 in case we want to pass more effects later
	const int color = 2; //vec4
}

namespace circleShaUni {

	// uniform locations for the Circle Renderer

	const int mvp = 0;
	const int screensize = 1;
}

class CircleRenderer : public ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/circle_vert.glsl";
	const char* GEOM_PATH = "data/shaders/circle_geom.glsl";
	const char* FRAG_PATH = "data/shaders/circle_frag.glsl";
	GLuint vao;
	ContainerVBO vbo;
	int count = 0;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 mvp;
	TextureImage tex1;
	TextureImage tex2;
public:
	bool init();
	bool createVAO();
	/**
	 * Transfers the host data to the device.
	 */
	void pushToDevice();
	void insertCircle(glm::vec2 position, float radius1, float radius2, glm::vec4 highlight, glm::vec4 color);
	void insertCircle(UIShapes::UICircle* circle);
	void insertHandler(UIHandler* handler);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize);
	//
	CircleRenderer();
	~CircleRenderer();
};

#endif