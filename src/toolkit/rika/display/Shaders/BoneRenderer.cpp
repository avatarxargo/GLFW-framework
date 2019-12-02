/**
 * Handles the shader for rendering character bones.
 *
 * @date: 4/8/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */


#include "BoneRenderer.h"

using namespace CharacterTools;

bool BoneRenderer::createVAO() {
	// TODO: error handling
	// Bind prog
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	//
	vbo.create();
	vbo.bind();
	// Set attributes
	int len = 32;
	glEnableVertexAttribArray(boneShaAtt::pos1);
	glVertexAttribPointer(boneShaAtt::pos1, 4, GL_FLOAT, false, len * sizeof(float), 0);
	glEnableVertexAttribArray(boneShaAtt::pos2);
	glVertexAttribPointer(boneShaAtt::pos2, 4, GL_FLOAT, false, len * sizeof(float), (void*)(4 * sizeof(float)) );
	glEnableVertexAttribArray(boneShaAtt::highlight);
	glVertexAttribPointer(boneShaAtt::highlight, 4, GL_FLOAT, false, len * sizeof(float), (void*)(8 * sizeof(float)) );
	glEnableVertexAttribArray(boneShaAtt::color);
	glVertexAttribPointer(boneShaAtt::color, 4, GL_FLOAT, false, len * sizeof(float), (void*)(12 * sizeof(float)) );
	//glEnableVertexAttribArray(boneShaAtt::transform);
	//glVertexAttribPointer(boneShaAtt::transform, 16, GL_FLOAT, false, len * sizeof(float), (void*)(16 * sizeof(float)) );
	// Set uniforms
	bindProgram();
	model = glm::mat4(1.0);
	view = glm::mat4(1.0);
	proj = glm::ortho(0.0f, 1200.0f, 0.0f, 800.0f, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(boneShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	glUseProgram(0);
	// Unbind everything (Let's be respectable of other shaders)
	glBindVertexArray(0);
	return false;
}


void BoneRenderer::clearVBO() {
	vbo.clear();
	count = 0;
}

void BoneRenderer::pushToDevice() {
	vbo.pushToDevice(GL_STREAM_DRAW);
}

void BoneRenderer::insertBone(Bone* bone) {
	Node* parent = bone->parent;
	Node* child = bone->child;
	vbo.insert(glm::vec4(parent->control.position, 0, 1));
	vbo.insert(glm::vec4(child->control.position, 0, 1));
	vbo.insert(glm::vec4(parent->control.radius1, child->control.radius1,0,0));
	vbo.insert(bone->color);
	//vbo.insert(bone->transform);
	vbo.insert(glm::vec4(0));
	vbo.insert(glm::vec4(0));
	vbo.insert(glm::vec4(0));
	vbo.insert(glm::vec4(0));
	//repeat for all children: ends when there are 0 children.
	for (int i = 0; i < child->children.size(); ++i) {
		Bone* bone = child->children[i];
		insertBone(bone);
	}
	count++;
}

void BoneRenderer::insertSkeleton(Skeleton* skeleton) {
	//insert edges
	Node* root = skeleton->root;
	for (int i = 0; i < root->children.size(); ++i) {
		Bone* bone = root->children[i];
		insertBone(bone);
	}

}

bool BoneRenderer::init() {
	setVertexShader((char*)VERT_PATH);
	setGeometryShader((char*)GEOM_PATH);
	setFragmentShader((char*)FRAG_PATH);
	return buildProgram();
}

void BoneRenderer::draw(glm::vec2 screensize) {
	//glUseProgram(shaderProgram); //moved to core
	bindProgram();
	glBindVertexArray(vao);
	//
	glUniform2fv(boneShaUni::screensize, 1, &screensize[0]);
	proj = glm::ortho(0.0f, screensize.x, 0.0f, screensize.y, -1.0f, 1.0f);
	mvp = model * view * proj;
	glUniformMatrix4fv(boneShaUni::mvp, 1, GL_FALSE, &mvp[0][0]);
	// Draw:
	glDrawArrays(GL_POINTS, 0, count);
	// Clean up
	glBindVertexArray(0);
	glUseProgram(0);
}

void BoneRenderer::deleteShaders() {
	deleteProgram();
}

BoneRenderer::BoneRenderer()
{
}


BoneRenderer::~BoneRenderer()
{
	deleteShaders();
}
