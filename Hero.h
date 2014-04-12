#ifndef HERO_H
#define HERO_H

#include <GL/glew.h>
#include <glm/vec2.hpp> 
#include "Tga.h"
#include "Common.h"
#include "Shader.h"
#include "Texture.h"
#include "Box.h"

struct Hero {
public:
	static const int height = 3;
	static const int width = 2;

	ShaderHero heroShader;
	GLuint heroVBO, heroVAO, heroIBO;
	GLuint hero_tex, hero_tex2;
	int cnt;

	Box box;

	glm::vec2 velocity;
	glm::vec2 position;

	static bool jumping;
	static bool moving;
	bool facingLeft;
	
	static float p_hero_vertices[];
	static unsigned int p_hero_indices[];	

	void init();
	void updateBox();
	void draw();

private:
		static bool isInit;
};

#endif
