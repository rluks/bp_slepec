#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>

#include <glm/vec2.hpp> 

#include "Box.h"

/**
 *	@brief struktura udrzuje informace o hernich urovnich
 */
struct Level {

	glm::vec2 heroStartPosition;	//pocatecni souradnice pro herni postavu
	std::string mapFilepath;		//cesta k texture
	Box exit;						//geometrie vychodu z urovne

	std::vector<glm::vec2> soundSourcePositions;	//souradnice pozic zvuku v urovni

	Level();
	Level(glm::vec2, std::string, Box);

	void addSoundSource(glm::vec2);
};

#endif