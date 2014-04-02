#ifndef Texture_H
#define Texture_H

#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <istream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include "OpenGL30Drv.h"
#include "Transform.h"
#include "Tga.h"

#include <windows.h>
#include <gl/gl.h>
#include "FreeImage.h"
#include <map>

#include "Common.h"

//from bmp
GLuint n_GLTextureFromBitmap(const TBmp *, GLenum n_internal_format);

GLuint LoadTexture(const char* filename,	//where to load the file from
		const unsigned int texID,			//arbitrary id you will reference the texture by
											//does not have to be generated with glGenTextures
		GLenum image_format = GL_RGB,		//format the image is in
		GLint internal_format = GL_RGB,		//format to store the image in
		GLint level = 0,					//mipmapping level
		GLint border = 0);					//border size

GLuint loadBMP_custom(const char * imagepath);

//red
GLuint n_GLTextureRFloatFromData();
GLuint n_GLTextureRFloat();
GLuint n_GLTextureRFloatDepthTest();

//red-green
GLuint n_GLTextureRGFloat();

//rgb
GLuint n_GLTextureRGBFloat();
GLuint n_GLTextureRGBFloatFromData();

#endif