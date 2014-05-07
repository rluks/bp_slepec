#ifndef SURFACE_H
#define SURFACE_H

#include <glm/vec3.hpp> 
#include "Shader.h"

/**
 *	@brief informace o povrchu (pouzije se pri vykreslovani hmatoveho vjemu)
 */
struct Surface {

	glm::vec3 color;	//barva krivky hmatoveho vjemu
	float frequency;	//frekvence
	float amplitude;	//amplituda

	ShaderSurface surfaceShader;	//shader povrchu, ktery se pouzije pri vykresleni
	float temperature;				//teplota povrchu (=rychlost kmitani)

	bool kills;	//je povrch smrtici?

	Surface();
	Surface(glm::vec3, float, float, ShaderSurface, float, bool);
};

#endif