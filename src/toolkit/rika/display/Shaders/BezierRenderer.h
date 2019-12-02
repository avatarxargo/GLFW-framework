/**
 * Draws bezier surfaces
 *
 * @date: 4/11/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef BEZIER_RENDERER_H
#define BEZIER_RENDERER_H

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

namespace bezierShaAtt {

	// attribute locations for the Circle Renderer

	const int boundingBox = 0; //upper left corner, bottom right corner
	const int guide1 = 1; //Guide 1 start, end
	const int guide2 = 2; //Guide 2 start, end
	const int highlight = 3; //vec4 in case we want to pass more effects later
	const int color = 4; //vec4
	//const int transform = 5;
	//5
	//6
	//7
}

namespace bezierShaUni {

	// uniform locations for the Circle Renderer

	const int mvp = 0;
	const int screensize = 1;
}

class BezierRenderer : public ShaderCore
{
private:
	const char* VERT_PATH = "data/shaders/bezier/bezier_vert.glsl";
	const char* GEOM_PATH = "data/shaders/bezier/bezier_geom.glsl";
	const char* FRAG_PATH = "data/shaders/bezier/bezier_frag.glsl";
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
	void insertShape(CharacterTools::BezierShape* shape);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize);
	//
	BezierRenderer();
	~BezierRenderer();
};

#endif