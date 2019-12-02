/**
 * Wraps a VBO (since we use it in a lot of shader in pretty much the same way)
 *
 * @date: 3/29/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef CONTAINER_VBO_H
#define CONTAINER_VBO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

class ContainerVBO {
private:
	GLuint vbo;
	std::vector<float> vboHost;
public:
	bool create();
	void bind();
	void clear();
	void insert(float value);
	void insert(glm::vec4 value);
	void insert(glm::mat4 value);
	int size();
	void pushToDevice(GLenum renderMode);
	GLuint getId() {
		return vbo;
	}
};

#endif