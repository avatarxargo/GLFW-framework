/**
 * Wraps a VBO (since we use it in a lot of shader in pretty much the same way)
 *
 * @date: 3/29/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "ContainerVBO.h"

bool ContainerVBO::create() {
	glGenBuffers(1, &vbo);
	return false;
}

void ContainerVBO::bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
}

void ContainerVBO::clear() {
	vboHost.clear();
}

void ContainerVBO::insert(float value) {
	vboHost.push_back(value);
}

void ContainerVBO::insert(glm::vec4 value) {
	vboHost.push_back(value.x);
	vboHost.push_back(value.y);
	vboHost.push_back(value.z);
	vboHost.push_back(value.w);
}

void ContainerVBO::insert(glm::mat4 value) {
	for(int i = 0; i < 16; ++i)
		insert(value[i]);
	/*insert(*((glm::vec4*)&value[0]));
	insert(*((glm::vec4*)&value[4]));
	insert(*((glm::vec4*)&value[8]));
	insert(*((glm::vec4*)&value[12]));*/
}

GLint ContainerVBO::size() {
	return vboHost.size();
}

void ContainerVBO::pushToDevice(GLenum renderMode) {
	/*if (vboHost.size() == 0)
		return;*/
	//printf("vbo %d\n", vboHost.size());
	// TODO: error handling
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vboHost.size() * sizeof(float), &vboHost[0], renderMode);
}