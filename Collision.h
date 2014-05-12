#ifndef COLLISION_H
#define COLLISION_H

#include <string>

#include <iostream>
#include <fstream>

#include <glm/vec2.hpp> 
#include "Surface.h"

/**
 *	@brief typ kolize (smer ve kterem objekty koliduji, kde A je hrac)
 */
enum collisionType {AB, BA, AoverB, AunderB};

/**
 *	@brief ulozeni informaci o vznikle kolizi
 */
struct Collision {

	collisionType type;	//typ kolize
	glm::vec2 point1;	//nejlevejsi, nejvyssi bod
	glm::vec2 point2;	//nejpravejsi, nejnizsi bod
	Surface surface;	//povrch kolidujiciho Box

	Collision();
	Collision(enum collisionType, glm::vec2, glm::vec2, Surface);
	std::string getTypeString() const;

	friend std::ostream& operator<<(std::ostream& os, const Collision& a);
};

#endif