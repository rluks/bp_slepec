#ifndef Physics_H
#define Physics_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp> 
#include "glm/gtx/string_cast.hpp"
#include <glm/detail/func_geometric.hpp>

#include "Common.h"
#include "Texture.h"
#include "Shader.h"

struct Box {
public:
	glm::vec2 vMin, vMax;
	std::string string();
	glm::vec2 project(glm::vec2);
};

struct BoxDebug {
public:
	GLuint VBO, VAO, IBO;
	std::vector<Box> boxes;
	ShaderBoxes boxesShader;

	//info o kolizich
	bool verticOver;

	void generate(TBmp *);
	void calculateTimeBasedMovement(DWORD, Keys, glm::vec2 *, glm::vec2 *);
	void SeparatingAxisTheorem(Box, glm::vec2 *);
	void draw();
	void drawBox(Box);
};

#endif
