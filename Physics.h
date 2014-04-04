#ifndef Physics_H
#define Physics_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp> 

#include "Common.h"
#include "Texture.h"
#include "Shader.h"

struct Box {
public:
	glm::vec2 vMin, vMax;
};

struct BoxDebug {
public:
	GLuint VBO, VAO, IBO;
	std::vector<Box> boxes;
	ShaderBoxes boxesShader;

	void generate(/*std::vector<Box> &boxes, */BMPFile * bmpfile);
	void draw();
};

#endif
