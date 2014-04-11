#ifndef COMMON_H
#define COMMON_H

#include <glm/vec2.hpp> 

#define datawidth 640
#define dataheight 480
// velikost dat


#define start_position_x 10.0
#define start_position_y 10.0
#define DEFAULT_GRAVITY 0.03f
#define DEFAULT_JUMP_VELOCITY -0.25
#define DEFAULT_WALK_VELOCITY 0.1f

/**
 *	@brief vraci "pointer" do VBO
 *	@param[in] off je offset v bytech, na ktery ma vraceny pointer ukazovat
 */
#define p_OffsetInVBO(off) ((void*)(off))

struct Keys {
public:
	bool up;
	bool down;
	bool left;
	bool right;
	bool space;

	bool reset;//num0
};


struct Box {
public:
	glm::vec2 vMin, vMax;
};

#endif