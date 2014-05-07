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
#include <windows.h>

#include "OpenGL30Drv.h"
#include "Transform.h"
#include "Tga.h"
#include "PNGLoad.h"

//#include <gl/gl.h>
//#include "FreeImage.h"
//#include <map>

#include "Common.h"

/*
 *	Funkce pro praci s texturami
 */

//bmp
GLuint n_GLTextureFromBitmap(const TBmp *, GLenum n_internal_format);

//png
GLuint LoadTextureFromPNGFile(const char *);

//tga
GLuint LoadTextureFromTGAFile(const char *);

//red
GLuint n_GLTextureRFloat();

//red - testovaci funkce
GLuint n_GLTextureRFloatFromData();
GLuint n_GLTextureRFloatDepthTest();

//red-green
GLuint n_GLTextureRGFloat();

//rgb
GLuint n_GLTextureRGBFloat();

//rgb - testovaci funkce
GLuint n_GLTextureRGBFloatFromData();

#endif