#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp> 
#include "glm/gtx/string_cast.hpp"
#include <glm/detail/func_geometric.hpp>
//#include <cmath>

#include "Common.h"
#include "Texture.h"
#include "Shader.h"
#include "Hero.h"

enum collisionType {AB, BA, AoverB, AunderB};

struct Collision {
public:
	collisionType type;
	glm::vec2 point1;
	glm::vec2 point2;

	Collision();
	Collision(enum collisionType, glm::vec2, glm::vec2);
};


struct BoxDebug {
public:
	GLuint VBO, VAO, IBO;
	std::vector<Box> boxes;
	ShaderBoxes boxesShader;
	ShaderPointSimple pointSimpleShader;

	glm::vec2 movementDirection;
	float gravity;

	//info o kolizich
	static bool standingOn;
	std::vector<Collision> collisions;

	void generate(TBmp *);
	void setMovementDirection(Keys);
	void calculateTimeBasedMovement(DWORD, Keys, Hero *);
	void SeparatingAxisTheorem(Hero *);

	void draw();
	void drawBox(Box);
	void drawCollisions();
};

#endif
