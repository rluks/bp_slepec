#ifndef BOX_H
#define BOX_H

#include <glm/vec2.hpp> 
#include "Surface.h"

/**
 *	@brief struktura udrzujici informace o geometrii
 */
struct Box {

	glm::vec2 vMin;	//levy horni vrchol 
	glm::vec2 vMax;	//pravy dolni vrchol
	Surface surface;//povrch objektu

	friend std::ostream& operator<<(std::ostream& os, const Box& a);

};

#endif