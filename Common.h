#ifndef Common_H
#define Common_H

#define datawidth 640
#define dataheight 480
// velikost dat


struct Keys {
public:
	bool up;
	bool down;
	bool left;
	bool right;
	bool space;
};

struct Hero {
public:
	static const int height = 3;
	static const int width = 2;
};

#endif