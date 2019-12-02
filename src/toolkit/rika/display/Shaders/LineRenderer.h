/**
 * Draws lines and arrows for editor tools
 *
 * @date: 4/12/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef LINE_RENDERER_H
#define LINE_RENDERER_H

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
#include "../Characters/CharacterTools.h"

namespace lineShaAtt {

	// attribute locations for the Circle Renderer

	const int pos = 0; //upper left corner, bottom right corner
	const int highlight = 1; //vec4 in case we want to pass more effects later
	const int color = 2; //vec4
}

class LineRenderer : public ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/line/line_vert.glsl";
	const char* GEOM_PATH = "data/shaders/line/line_geom.glsl";
	const char* FRAG_PATH = "data/shaders/line/line_frag.glsl";
	GLuint vao;
	ContainerVBO vbo;
	vector<GLint> lineStarts;
	vector<GLint> lineLengths;
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
	void insertSingleLine(UIShapes::UILine line);
	void insertLine(UIShapes::UILineStrip line);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize);
	//
	LineRenderer();
	~LineRenderer();
};

#endif