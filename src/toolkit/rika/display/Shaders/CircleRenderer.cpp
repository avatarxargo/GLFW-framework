/**
 * Handles the shader for rendering circles.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "CircleRenderer.h"

bool CircleRenderer::createVAO() {
	// TODO: error handling
	// Bind prog
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//
	vbo.create();
	vbo.bind();
	// Set attributes
	int len = 12;
	glEnableVertexAttribArray(circleShaAtt::posrad);
	glVertexAttribPointer(circleShaAtt::posrad, 4, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(circleShaAtt::highlight);
	glVertexAttribPointer(circleShaAtt::highlight, 4, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(circleShaAtt::color);
	glVertexAttribPointer(circleShaAtt::color, 4, GL_FLOAT, false, len * sizeof(float), (void*)(8 * sizeof(float)));
	// Set uniforms
	bindProgram();
	model = glm::mat4(1.0);
	view = glm::mat4(1.0);
	proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(circleShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	glUseProgram(0);
	// Unbind everything (Let's be respectable of other shaders)
	glBindVertexArray(0);
	return false;
}

void CircleRenderer::insertCircle(glm::vec2 position, float radius1, float radius2, glm::vec4 highlight, glm::vec4 color) {
	vbo.insert(position.x);
	vbo.insert(position.y);
	vbo.insert(radius1);
	vbo.insert(radius2);
	//
	vbo.insert(highlight);
	//
	vbo.insert(color);
	count++;
}

void CircleRenderer::insertCircle(UIShapes::UICircle* circle) {
	vbo.insert(circle->position.x);
	vbo.insert(circle->position.y);
	vbo.insert(circle->radius1);
	vbo.insert(circle->radius2);
	//
	vbo.insert(circle->highlight.x);
	vbo.insert(circle->highlight.y);
	vbo.insert(circle->highlight.z);
	vbo.insert(circle->highlight.w);
	//
	vbo.insert(circle->color.x);
	vbo.insert(circle->color.y);
	vbo.insert(circle->color.z);
	vbo.insert(circle->color.w);
	count++;
}

void CircleRenderer::insertHandler(UIHandler* handler) {
	for (int i = 0; i < handler->circles.size(); ++i) {
		insertCircle(handler->circles[i]);
	}
	// other shapes?
}

void CircleRenderer::clearVBO() {
	vbo.clear();
	count = 0;
}

void CircleRenderer::pushToDevice() {
	vbo.pushToDevice(GL_STREAM_DRAW);
}

bool CircleRenderer::init() {
	setVertexShader((char*)VERT_PATH);
	setGeometryShader((char*)GEOM_PATH);
	setFragmentShader((char*)FRAG_PATH);
	return buildProgram();
}

void CircleRenderer::draw(glm::vec2 screensize) {
	//glUseProgram(shaderProgram); //moved to core
	bindProgram();
	glBindVertexArray(vao);
	//
	glUniform2fv(circleShaUni::screensize, 1, &screensize[0]);
	proj = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(circleShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	// Draw:
	glDrawArrays(GL_POINTS, 0, count);
	// Clean up
	glBindVertexArray(0);
	glUseProgram(0);
}

void CircleRenderer::deleteShaders() {
	deleteProgram();
}

CircleRenderer::CircleRenderer()
{
}


CircleRenderer::~CircleRenderer()
{
	deleteShaders();
}
