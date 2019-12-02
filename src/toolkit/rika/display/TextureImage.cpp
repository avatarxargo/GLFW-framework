/**
 * Loads image from file and uploads it to GPU.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "TextureImage.h"

void TextureImage::load(const char* path, bool instantGPU) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(path, 0);
	bitmap = FreeImage_Load(format, path);
	hasAlpha = FreeImage_IsTransparent(bitmap);
	width = FreeImage_GetWidth(bitmap);
	height = FreeImage_GetHeight(bitmap);
	if (instantGPU) {
		uploadToGPU();
	}
}

GLuint TextureImage::uploadToGPU() {
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	int w = FreeImage_GetWidth(bitmap);
	int h = FreeImage_GetHeight(bitmap);
	// Set clamping behavior
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //GL_CLAMP_TO_BORDER
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Copy data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, FreeImage_GetBits(FreeImage_ConvertTo32Bits(bitmap)));
	FreeImage_Unload(bitmap);
	return texid;
}

GLuint TextureImage::getID() {
	return texid;
}

void TextureImage::clear() {
	glDeleteTextures(1, &texid);
}

TextureImage::TextureImage()
{
}


TextureImage::~TextureImage()
{
}
