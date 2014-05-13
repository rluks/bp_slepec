#ifndef HERO_H
#define HERO_H

#include <GL/glew.h>
#include <glm/vec2.hpp> 
#include "Tga.h"
#include "Common.h"
#include "Shader.h"
#include "Texture.h"
#include "Box.h"

/**
 *	@brief funkce a promenne pro praci s herni postavou (vc. vykreslovani)
 */
struct Hero {
public:
	static const int height;
	static const float width;
	//vychozi rozmery pro Box

	Box box;
	//geometrie

	ShaderHero heroShader;
	GLuint heroVBO, heroVAO, heroIBO;
	GLuint hero_tex, hero_tex2;
	GLuint hero_tex_jump;
	static float p_hero_vertices[];
	static unsigned int p_hero_indices[];	
	int cnt;
	//vykreslovani

	glm::vec2 velocity;
	glm::vec2 position;
	static bool jumping;
	static bool moving;
	bool facingLeft;
	//ovladani a pohyb

	void init();
	void updateBox();
	void draw();

	void cleanupOpenGL();
	//maze opengl objekty

private:
	static bool isInit;
};

#endif
