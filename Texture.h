#ifndef TEXTURE_H
#define TEXTURE_H

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
#include "PNGLoad.h"

#include <windows.h>
#include <gl/gl.h>
#include "FreeImage.h"
#include <map>

#include "Common.h"

struct BMPFile {
public:
	unsigned int imageSize;
	unsigned int n_width, n_height;
	unsigned char * p_buffer;
};

//from bmp
GLuint n_GLTextureFromBitmap(const TBmp *, GLenum n_internal_format);

GLuint loadBMP_custom(const char * imagepath);
int loadBMP_customData(const char * imagepath, TBmp * bmpfile);

GLuint LoadTextureFromPNGFile(const char *);
GLuint LoadTextureFromTGAFile(const char *);

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