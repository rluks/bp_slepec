#ifndef Hero_H
#define Hero_H

#include <GL/glew.h>
#include <glm/vec2.hpp> 
#include "Tga.h"
#include "Common.h"
#include "Shader.h"
#include "Texture.h"

struct Hero {
public:
	static const int height = 3;
	static const int width = 2;

	ShaderHero heroShader;
	GLuint hero_tex;

	void init();
	void draw(glm::vec2);
};

#endif
