/**
 * Draws lines and arrows
 *
 * @date: 4/12/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "LineRenderer.h"

using namespace CharacterTools;

bool LineRenderer::createVAO() {
	// TODO: error handling
	// Bind prog
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//
	vbo.create();
	vbo.bind();
	// Set attributes
	int len = 12;
	glEnableVertexAttribArray(lineShaAtt::pos);
	glVertexAttribPointer(lineShaAtt::pos, 4, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(lineShaAtt::highlight);
	glVertexAttribPointer(lineShaAtt::highlight, 4, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(lineShaAtt::color);
	glVertexAttribPointer(lineShaAtt::color, 4, GL_FLOAT, false, len * sizeof(float), (void*)(8 * sizeof(float)));
	// Set uniforms
	bindProgram();
	model = glm::mat4(1.0);
	view = glm::mat4(1.0);
	proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(universalShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	glUseProgram(0);
	// Unbind everything (Let's be respectable of other shaders)
	glBindVertexArray(0);
	return false;
}


void LineRenderer::clearVBO() {
	vbo.clear();
	lineStarts.clear();
	lineLengths.clear();
	count = 0;
}

void LineRenderer::pushToDevice() {
	vbo.pushToDevice(GL_STREAM_DRAW);
}

void LineRenderer::insertSingleLine(UIShapes::UILine line) {
	lineStarts.push_back(vbo.size());
	lineLengths.push_back(2);
	count++;
	vbo.insert(line.from);
	vbo.insert(line.highlight);
	vbo.insert(line.color);
	vbo.insert(line.to);
	vbo.insert(line.highlight);
	vbo.insert(line.color);
}

bool LineRenderer::init() {
	clearVBO();
	setVertexShader((char*)VERT_PATH);
	setGeometryShader((char*)GEOM_PATH);
	setFragmentShader((char*)FRAG_PATH);
	return buildProgram();
}

void LineRenderer::draw(glm::vec2 screensize) {
	//glUseProgram(shaderProgram); //moved to core
	bindProgram();
	glBindVertexArray(vao);
	//
	glLineWidth(5);
	glUniform2fv(universalShaUni::screensize, 1, &screensize[0]);
	proj = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(universalShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	// Draw:
	glMultiDrawArrays(GL_LINE_LOOP, &lineStarts[0], &lineLengths[0], count);
	// Clean up
	glBindVertexArray(0);
	glUseProgram(0);
}

void LineRenderer::deleteShaders() {
	deleteProgram();
}

LineRenderer::LineRenderer()
{
}


LineRenderer::~LineRenderer()
{
	deleteShaders();
}
