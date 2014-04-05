#ifndef Common_H
#define Common_H

#define datawidth 640
#define dataheight 480
// velikost dat

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
};

#endif