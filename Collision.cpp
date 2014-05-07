#include "Collision.h"

Collision::Collision()
{
}

Collision::Collision(enum collisionType collisType, glm::vec2 p1, glm::vec2 p2, Surface s)
:type(collisType), point1(p1), point2(p2), surface(s)
{
}

/**
 *	@brief vrati retezec odpovidajici danemu typu kolize
 */
std::string Collision::getTypeString(){

	std::string typeString;

	if(type == AB)
		typeString = "AB";
	else if(type == BA)
		typeString = "BA";
	else if(type == AoverB)
		typeString = "A nad B";
	else if(type == AunderB)
		typeString = "A pod B";
	else 
		typeString = "neznamy type kolize";

	return typeString;
}