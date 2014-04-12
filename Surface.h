#ifndef SURFACE_H
#define SURFACE_H

#include <glm/vec3.hpp> 

struct Surface {
public:
	glm::vec3 color;
	float frequency;
	float amplitude;
};

#endif