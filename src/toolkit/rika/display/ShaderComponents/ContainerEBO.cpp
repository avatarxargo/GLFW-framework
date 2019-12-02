/**
 * Wraps a EBO (since we use it in a lot of shader in pretty much the same way)
 *
 * @date: 4/8/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "ContainerEBO.h"

bool ContainerEBO::create() {
	glGenBuffers(1, &ebo);
	return false;
}

void ContainerEBO::bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}

void ContainerEBO::clear() {
	eboHost.clear();
}

void ContainerEBO::insert(GLuint value) {
	eboHost.push_back(value);
}

void ContainerEBO::pushToDevice(GLenum renderMode) {
	// TODO: error handling
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, eboHost.size() * sizeof(GLuint), &eboHost[0], renderMode);
}