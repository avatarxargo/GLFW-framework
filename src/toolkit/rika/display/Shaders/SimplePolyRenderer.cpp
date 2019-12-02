/**
 * Handles the shader for rendering circles.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "SimplePolyRenderer.h"

bool SimplePolyRenderer::createVAO() {
	// TODO: error handling
	// Bind prog
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//
	vbo.create();
	vbo.bind();
	//
	ebo.create();
	ebo.bind();
	// Set attributes
	int len = 10;
	glEnableVertexAttribArray(simplePolyShaAtt::posrad);
	glVertexAttribPointer(simplePolyShaAtt::posrad, 4, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(simplePolyShaAtt::highlight);
	glVertexAttribPointer(simplePolyShaAtt::highlight, 4, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(simplePolyShaAtt::uv);
	glVertexAttribPointer(simplePolyShaAtt::uv, 2, GL_FLOAT, false, len * sizeof(float), (void*)(8 * sizeof(float)));
	// Set uniforms
	bindProgram();
	model = glm::mat4(1.0);
	view = glm::mat4(1.0);
	proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(simplePolyShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	glUseProgram(0);
	// Unbind everything (Let's be respectable of other shaders)
	glBindVertexArray(0);
	return false;
}

void SimplePolyRenderer::insertVertex(glm::vec3 position, glm::vec4 highlight, glm::vec2 uv) {
	vbo.insert(position.x);
	vbo.insert(position.y);
	vbo.insert(position.z);
	vbo.insert(0.0f);
	//
	vbo.insert(highlight);
	//
	vbo.insert(uv.x);
	vbo.insert(uv.y);
	count++;
}

void SimplePolyRenderer::insertElement(UIShapes::UIElement* element) {
	for (int i = 0; i < element->elements.size(); ++i) {
		ebo.insert(element->elements[i]);
	}
}

void SimplePolyRenderer::clearVBO() {
	vbo.clear();
	count = 0;
}

void SimplePolyRenderer::pushToDevice() {
	vbo.pushToDevice(GL_STREAM_DRAW);
}

void SimplePolyRenderer::pushEBOToDevice() {
	ebo.pushToDevice(GL_STATIC_DRAW);
}

bool SimplePolyRenderer::init() {
	setVertexShader((char*)VERT_PATH);
	setFragmentShader((char*)FRAG_PATH);
	return buildProgram();
}

void SimplePolyRenderer::draw(glm::vec2 screensize) {
	//glUseProgram(shaderProgram); //moved to core
	bindProgram();
	glBindVertexArray(vao);
	//
	glUniform2fv(simplePolyShaUni::screensize, 1, &screensize[0]);
	proj = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(simplePolyShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	// Draw:
	glDrawArrays(GL_POINTS, 0, count);
	// Clean up
	glBindVertexArray(0);
	glUseProgram(0);
}

void SimplePolyRenderer::deleteShaders() {
	deleteProgram();
}

SimplePolyRenderer::SimplePolyRenderer()
{
}


SimplePolyRenderer::~SimplePolyRenderer()
{
	deleteShaders();
}
