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

	void generate(TBmp *);
	void SeparatingAxisTheorem(Box);
	void draw();
	void drawBox(Box);
};

#endif
