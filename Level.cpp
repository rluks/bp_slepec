#include "Level.h"


Level::Level(){
}

Level::Level(glm::vec2 start, std::string filepath, Box exitBox)
:heroStartPosition(start), mapFilepath(filepath), exit(exitBox)
{
}

void Level::addSoundSource(glm::vec2 source){
	soundSourcePositions.push_back(source);
}