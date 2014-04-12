#ifndef BOX_H
#define BOX_H

#include <glm/vec2.hpp> 
#include "Surface.h"

struct Box {
public:
	glm::vec2 vMin, vMax;
	Surface surface;
};

#endif