#include "Level.h"


Level::Level()
{
}

Level::Level(glm::vec2 start, std::string filepath, Box exitBox)
:heroStartPosition(start), mapFilepath(filepath), exit(exitBox)
{
}

/**
 *	@brief prida souradnice zdroje zvuku do vektoru zdroju
 *	@param[in] source souradnice zdroje
 */
void Level::addSoundSource(glm::vec2 source){
	soundSourcePositions.push_back(source);
}