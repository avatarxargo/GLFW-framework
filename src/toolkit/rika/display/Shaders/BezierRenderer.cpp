/**
 * Draws bezier surfaces
 *
 * @date: 4/11/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "BezierRenderer.h"

using namespace CharacterTools;

bool BezierRenderer::createVAO() {
	// TODO: error handling
	// Bind prog
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//
	vbo.create();
	vbo.bind();
	// Set attributes
	int len = 5*4;
	glEnableVertexAttribArray(bezierShaAtt::boundingBox);
	glVertexAttribPointer(bezierShaAtt::boundingBox, 4, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(bezierShaAtt::guide1);
	glVertexAttribPointer(bezierShaAtt::guide1, 4, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(bezierShaAtt::guide2);
	glVertexAttribPointer(bezierShaAtt::guide2, 4, GL_FLOAT, false, len * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(bezierShaAtt::highlight);
	glVertexAttribPointer(bezierShaAtt::highlight, 4, GL_FLOAT, false, len * sizeof(float), (void*)(12 * sizeof(float)));
	glEnableVertexAttribArray(bezierShaAtt::color);
	glVertexAttribPointer(bezierShaAtt::color, 4, GL_FLOAT, false, len * sizeof(float), (void*)(16 * sizeof(float)));
	//glEnableVertexAttribArray(boneShaAtt::transform);
	//glVertexAttribPointer(boneShaAtt::transform, 16, GL_FLOAT, false, len * sizeof(float), (void*)(16 * sizeof(float)) );
	// Set uniforms
	bindProgram();
	model = glm::mat4(1.0);
	view = glm::mat4(1.0);
	proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(bezierShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	glUseProgram(0);
	// Unbind everything (Let's be respectable of other shaders)
	glBindVertexArray(0);
	return false;
}


void BezierRenderer::clearVBO() {
	vbo.clear();
	lineStarts.clear();
	lineLengths.clear();
	count = 0;
}

void BezierRenderer::pushToDevice() {
	vbo.pushToDevice(GL_STREAM_DRAW);
}

void BezierRenderer::insertShape(CharacterTools::BezierShape* shape) {
	lineStarts.push_back(vbo.size());
	lineLengths.push_back(0);
	for (int i = 0; i < shape->nodes.size(); ++i) {
		vbo.insert(glm::vec4(shape->nodes[i]->pos->position,0,1));
		vbo.insert(glm::vec4(shape->nodes[i]->guideIn->position, 0, 1));
		vbo.insert(glm::vec4(shape->nodes[i]->guideOut->position, 0, 1));
		vbo.insert(shape->nodes[i]->highlight);
		vbo.insert(shape->nodes[i]->color);
		lineLengths[count]++;
	}
	//After we are done refering to count as an index we increase it.
	count++;
}

bool BezierRenderer::init() {
	clearVBO();
	setVertexShader((char*)VERT_PATH);
	setGeometryShader((char*)GEOM_PATH);
	setFragmentShader((char*)FRAG_PATH);
	return buildProgram();
}

void BezierRenderer::draw(glm::vec2 screensize) {
	//glUseProgram(shaderProgram); //moved to core
	bindProgram();
	glBindVertexArray(vao);
	//
	glLineWidth(5);
	glUniform2fv(bezierShaUni::screensize, 1, &screensize[0]);
	proj = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(bezierShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	// Draw:
	glMultiDrawArrays(GL_LINE_LOOP, &lineStarts[0], &lineLengths[0], count);
	// Clean up
	glBindVertexArray(0);
	glUseProgram(0);
}

void BezierRenderer::deleteShaders() {
	deleteProgram();
}

BezierRenderer::BezierRenderer()
{
}


BezierRenderer::~BezierRenderer()
{
	deleteShaders();
}
