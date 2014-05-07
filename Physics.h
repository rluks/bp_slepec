#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp> 

#include "Common.h"
#include "Texture.h"
#include "Shader.h"
#include "Hero.h"
#include "Box.h"
#include "Collision.h"

/**
 *	@brief prace s geometrii, kolizemi a pohybem
 */
struct BoxDebug {

	//geometrie
	std::vector<Box> boxes;

	//vykreslovani
	GLuint VBO, VAO, IBO;
	ShaderBoxes boxesShader;
	ShaderPointSimple pointSimpleShader;

	//hrac
	glm::vec2 movementDirection;
	float gravity;

	//info o kolizich
	static bool standingOn;
	std::vector<Collision> collisions;

	//pro hmatovy vjem
	std::vector<float> pozice1D;
	GLuint tactileVBO, tactileVAO;
	ShaderSurface surfaceShader, surfaceSinus, surfaceZigzag;
	float phase;

	//funkce
	void generate(TBmp *);
	void setMovementDirection(Keys);
	void calculateTimeBasedMovement(DWORD, Keys, Hero *);
	void SeparatingAxisTheorem(Hero *);

	//vykreslovaci funkce
	void draw();
	void drawBox(Box, glm::vec3);
	void drawCollisions();
	void drawCollisionsPoints();

	//maze opengl objekty
	void cleanupOpenGL();
};

#endif
