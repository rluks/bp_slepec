#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>

#include <glm/vec2.hpp> 

#include "Box.h"

struct Level {
public:
	glm::vec2 heroStartPosition;
	std::string mapFilepath;
	Box exit;

	std::vector<glm::vec2> soundSourcePositions;

	Level();
	Level(glm::vec2, std::string, Box);
	void addSoundSource(glm::vec2);
};

#endif