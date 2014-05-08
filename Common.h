#ifndef COMMON_H
#define COMMON_H

#include <glm/vec2.hpp> 

#define datawidth 640
#define dataheight 480
// velikost dat

#define MAP_SIZE_X 32.0f
#define MAP_SIZE_Y 24.0f

#define DEFAULT_GRAVITY 0.03f
#define DEFAULT_JUMP_VELOCITY -0.33f
#define DEFAULT_WALK_VELOCITY 0.1f
// definice vychozich hodnot nekterych promennych

/**
 *	@brief vraci "pointer" do VBO
 *	@param[in] off je offset v bytech, na ktery ma vraceny pointer ukazovat
 */
#define p_OffsetInVBO(off) ((void*)(off))

/**
 *	@brief informace o stisklych klavesach
 */
struct Keys {

	/* kurzorove sipky */
	bool up;
	bool down;
	bool left;
	bool right;

	/* ostatni */
	bool space;
	bool reset;	//num0
};

#endif