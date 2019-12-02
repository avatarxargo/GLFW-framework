/**
 * Shader for drawing straightforward EBOed polygons with uv coordinates for good measure. No facny effects. No geometry shader.
 *
 * @date: 6/11/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef SIMPLE_POLY_RENDERER_H
#define SIMPLE_POLY_RENDERER_H

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
#include "../ShaderComponents/ContainerEBO.h"
#include "../ShaderComponents/ShaderCore.h"
#include "../UI/UISprite.h"

namespace simplePolyShaAtt {

	// attribute locations for the Circle Renderer

	const int posrad = 0; //posx posy, r1 - outer, r2 - inner
	const int highlight = 1; //vec4 in case we want to pass more effects later
	const int uv = 2; //uv
}

namespace simplePolyShaUni {

	// uniform locations for the Circle Renderer

	const int mvp = 0;
	const int screensize = 1;
}

class SimplePolyRenderer : public ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/poly_vert.glsl";
	const char* GEOM_PATH = "";
	const char* FRAG_PATH = "data/shaders/poly_frag.glsl";
	GLuint vao;
	ContainerVBO vbo;
	ContainerEBO ebo;
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
	void pushEBOToDevice();
	void insertVertex(glm::vec3 position, glm::vec4 highlight, glm::vec2 uv);
	void insertElement(UIShapes::UIElement* element);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize);
	//
	SimplePolyRenderer();
	~SimplePolyRenderer();
};

#endif