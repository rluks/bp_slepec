#include "Collision.h"

Collision::Collision(){
}

Collision::Collision(enum collisionType collisType, glm::vec2 p1, glm::vec2 p2) : type(collisType), point1(p1), point2(p2)
{

}

std::string Collision::getTypeString(){

	std::string typeString;

	if(type == AB)
		typeString = "AB";
	else if(type == BA)
		typeString = "BA";
	else if(type == AoverB)
		typeString = "A nadB";
	else if(type == AunderB)
		typeString = "Apod B";
	else 
		typeString = "A je nekde kolem B ale nevim type kolize";

	return typeString;
}