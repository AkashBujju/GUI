#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

unsigned int make_texture(const char* fname);

unsigned int make_texture(const char* fname)
{
	 stbi_set_flip_vertically_on_load(true);
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fname, &width, &height, &nrChannels, 0);
	 
    if (data)
    {
		 glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		 glGenerateMipmap(GL_TEXTURE_2D);
	 }
	 else
	 {
		 std::cout << "Failed to load texture" << std::endl;
	 }
	 stbi_image_free(data);

	 return texture;
}

#endif
