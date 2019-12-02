/**
 * Handles the shader for rendering various basic geometric shapes (mainly for the editor and developer view).
 *
 * @date: 3/29/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef SHAPERENDERER_H
#define SHAPERENDERER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/*
 * Shapes to draw:
 * Rectangle: x,y from, w,h to.
 * Circle: x,y centre, r radius.
 * Line: x,y x,y from, x,y to.
 * =
 * type int
 * vec4 params
 * vec4 color
 */

namespace shaShaAtt {

	// attribute locations for the Sprite Renderer

	const int type = 0;
	const int paramsA = 1;
	const int paramsB = 1; //for good measure now. It will probably come in handy later.
	const int color = 2;
}

namespace shaShaUni {

	// uniform locations for the Sprite Renderer

	const int mvp = 0;
	const int screensize = 1;
}

class ShapeRenderer
{
private:
	const char* VERT_PATH = "data/shaders/shape_vert.glsl";
	const char* GEOM_PATH = "data/shaders/shape_geom.glsl";
	const char* FRAG_PATH = "data/shaders/shape_frag.glsl";
	GLuint shaderProgram;
	GLuint vertexShader, geometryShader, fragmentShader;
	GLuint vao, vbo;
	std::vector<float> vboHost;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 mvp;
public:
	bool init();
	bool createVAO();
	void pushToDevice();
	void drawLine(float x1, float y1, float x2, float y2);
	void drawTriangle(float x1, float y1, float x2, float y2, float x3, float y3);
	void drawRect(float x, float y, float w, float h);
	void drawCircle(float x, float y, float r);
	void clearVBO();
	void deleteShaders();
	void draw(glm::vec2 screensize);
	//
	ShapeRenderer();
	~ShapeRenderer();
};

#endif