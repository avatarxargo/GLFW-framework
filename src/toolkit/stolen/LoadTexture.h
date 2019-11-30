#ifndef __LOADTEXTURE_H__
#define __LOADTEXTURE_H__

#include <string>
#include <windows.h>
#include "GL/glew.h"
#include "GL/gl.h"
#include <GLFW/glfw3.h>

GLuint LoadTexture(const std::string& fname);
GLFWimage LoadGLFWImage(const std::string& fname);


#endif