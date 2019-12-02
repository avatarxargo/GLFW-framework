/**
 * Loads image from file and uploads it to GPU.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef TEXTUREIMAGE_H
#define TEXTUREIMAGE_H

#include <FreeImage.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

class TextureImage
{
private:
	bool hasAlpha;
	int width, height;
	GLuint texid;
	FIBITMAP *bitmap;
public:
	TextureImage();
	void load(const char* path, bool instantGPU);
	void clear();
	GLuint uploadToGPU();
	GLuint getID();
	~TextureImage();
};

#endif
