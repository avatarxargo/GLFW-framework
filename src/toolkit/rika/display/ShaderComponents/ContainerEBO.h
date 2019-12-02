/**
 * Wraps a EBO (since we use it in a lot of shader in pretty much the same way)
 *
 * @date: 4/8/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef CONTAINER_EBO_H
#define CONTAINER_EBO_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>

class ContainerEBO {
private:
	GLuint ebo;
	std::vector<GLuint> eboHost;
public:
	bool create();
	void bind();
	void clear();
	void insert(GLuint value);
	void pushToDevice(GLenum renderMode);
	GLuint getId() {
		return ebo;
	}
};

#endif