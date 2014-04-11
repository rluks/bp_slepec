#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp> 
#include "glm/gtx/string_cast.hpp"
#include <glm/detail/func_geometric.hpp>

#include "Common.h"
#include "Texture.h"
#include "Shader.h"
#include "Hero.h"



struct BoxDebug {
public:
	GLuint VBO, VAO, IBO;
	std::vector<Box> boxes;
	ShaderBoxes boxesShader;

	glm::vec2 movementDirection;
	//info o kolizich
	bool verticOver;

	void generate(TBmp *);
	void setMovementDirection(Keys);
	void calculateTimeBasedMovement(DWORD, Keys, Hero *);
	void SeparatingAxisTheorem(Box, glm::vec2 *);
	void draw();
	void drawBox(Box);
};

#endif
