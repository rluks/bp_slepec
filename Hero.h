#ifndef HERO_H
#define HERO_H

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
	GLuint heroVBO, heroVAO, heroIBO;
	GLuint hero_tex;

	Box box;
	static bool jumping;
	static bool moving;
	
	static float p_hero_vertices[];
	static unsigned int p_hero_indices[];	

	void init();
	void updateBox(glm::vec2);
	void draw();

private:
		static bool isInit;
};

#endif
