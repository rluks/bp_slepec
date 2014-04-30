#include "Hero.h"

float Hero::p_hero_vertices[20] = {0, 0, 0, 0, 1, /**/1, 0, width, 0, 1,/**/ 1, 1, width, height, 1,/**/ 0, 1, 0, height, 1};
unsigned int Hero::p_hero_indices[6] = {0, 1, 2, 0, 3, 2};	
bool Hero::isInit = false;
bool Hero::jumping = false;
bool Hero::moving = false;

void Hero::init(){

	box.vMin.x = 0;
	box.vMin.y = 0;
	box.vMax.x = box.vMin.x + width;
	box.vMax.y = box.vMin.y + height;

	heroShader.compile();
	hero_tex = LoadTextureFromPNGFile("Textures/player1.png");
	hero_tex2 = LoadTextureFromPNGFile("Textures/player0.png");

	glGenBuffers(1, &heroVBO);
	glBindBuffer(GL_ARRAY_BUFFER, heroVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_hero_vertices), p_hero_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &heroIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heroIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_hero_indices), p_hero_indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &heroVAO);
	glBindVertexArray(heroVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, heroVBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), p_OffsetInVBO(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), p_OffsetInVBO(2 * sizeof(float)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, heroIBO);
	}
	glBindVertexArray(0);

	cnt = 0;
	isInit = true;
}

void Hero::updateBox(){
	box.vMin.x = position.x;
	box.vMin.y = position.y;
	box.vMax.x = box.vMin.x + width;
	box.vMax.y = box.vMin.y + height;
}

void Hero::draw(){

	try{	
		if(!isInit) throw "Error - Hero:: not initialized";		
	} catch (const char* msg){
		std::cerr << msg << std::endl;
		return;
	}

	if(velocity.x > 0.0 || velocity.x < 0.0){
		cnt++;
		if(cnt < 20)
			glBindTexture(GL_TEXTURE_2D, hero_tex);
		else if(cnt < 40)
			glBindTexture(GL_TEXTURE_2D, hero_tex2);
		else
			cnt = 0;
	}else
		glBindTexture(GL_TEXTURE_2D, hero_tex);

	heroShader.bind(box.vMin, facingLeft);

	glBindVertexArray(heroVAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}