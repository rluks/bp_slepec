#ifndef PHYSICS_H
#define PHYSICS_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec2.hpp> 
//#include "glm/gtx/string_cast.hpp"
//#include <glm/detail/func_geometric.hpp>

#include "Common.h"
#include "Texture.h"
#include "Shader.h"
#include "Hero.h"
#include "Box.h"
#include "Collision.h"




struct BoxDebug {
public:
	GLuint VBO, VAO, IBO;
	std::vector<Box> boxes;
	ShaderBoxes boxesShader;
	ShaderPointSimple pointSimpleShader;
	ShaderCurve curveShader;

	glm::vec2 movementDirection;
	float gravity;

	//info o kolizich
	static bool standingOn;
	std::vector<Collision> collisions;

	std::vector<float> pozice1D;
	GLuint tactileVBO, tactileVAO;

	void generate(TBmp *);
	void setMovementDirection(Keys);
	void calculateTimeBasedMovement(DWORD, Keys, Hero *);
	void SeparatingAxisTheorem(Hero *);

	void draw();
	void drawBox(Box);
	void drawCollisions();
	void drawCollisionsPoints();
};

#endif
