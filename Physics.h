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

struct BoxDebug {
public:
	//geometrie
	GLuint VBO, VAO, IBO;
	std::vector<Box> boxes;
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
	ShaderSurface surfaceShader, surfaceShader2, surfaceZigzag;
	float phase;

	//funkce
	void generate(TBmp *);
	void setMovementDirection(Keys);
	void calculateTimeBasedMovement(DWORD, Keys, Hero *);
	void SeparatingAxisTheorem(Hero *);

	//vykreslovaci funkce
	void draw();
	void drawBox(Box);
	void drawCollisions();
	void drawCollisionsPoints();
};

#endif
