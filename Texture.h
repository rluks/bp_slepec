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

#include "Common.h"

GLuint n_GLTextureFromBitmap(const TBmp *, GLenum n_internal_format);
GLuint n_GLTextureRFloatFromData();
GLuint n_GLTextureRFloat();
GLuint n_GLTextureRGFloat();
GLuint n_GLTextureRGBFloat();