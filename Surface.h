#ifndef SURFACE_H
#define SURFACE_H

#include <glm/vec3.hpp> 
#include "Shader.h"

struct Surface {
public:
	glm::vec3 color;
	float frequency;
	float amplitude;
	ShaderSurface surfaceShader;
};

#endif