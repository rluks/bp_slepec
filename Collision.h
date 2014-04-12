#ifndef COLLISION_H
#define COLLISION_H

#include <string>
#include <glm/vec2.hpp> 
#include "Surface.h"

enum collisionType {AB, BA, AoverB, AunderB};

struct Collision {
public:
	collisionType type;
	glm::vec2 point1;
	glm::vec2 point2;
	Surface surface;

	Collision();
	Collision(enum collisionType, glm::vec2, glm::vec2, Surface);
	std::string getTypeString();
};

#endif