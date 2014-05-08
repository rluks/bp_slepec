#include "Hero.h"

//vykreslovani
float Hero::p_hero_vertices[20] = {0, 0, 0, 0, 1, /**/1, 0, width, 0, 1,/**/ 1, 1, width, height, 1,/**/ 0, 1, 0, height, 1};
unsigned int Hero::p_hero_indices[6] = {0, 1, 2, 0, 3, 2};	

//kontrola inicializace
bool Hero::isInit = false;

//pohyb
bool Hero::jumping = false;
bool Hero::moving = false;

/**
 *	@brief inicializacni funkce, musi byt zavolana pred pouzitim dalsich funkci!
 */
void Hero::init(){

	//Box
	box.vMin.x = 0;
	box.vMin.y = 0;
	box.vMax.x = box.vMin.x + width;
	box.vMax.y = box.vMin.y + height;

	//shader
	heroShader.compile();

	//textury
	hero_tex = LoadTextureFromPNGFile("Textures/player1.png");
	hero_tex2 = LoadTextureFromPNGFile("Textures/player0.png");
	hero_tex_jump = LoadTextureFromPNGFile("Textures/player_jump.png");

	//inicializace vbo, vao, ibo
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

/**
 *	@brief obnovi hodnoty v geometrii podle aktualizovane pozice
 */
void Hero::updateBox(){
	box.vMin.x = position.x;
	box.vMin.y = position.y;
	box.vMax.x = box.vMin.x + width;
	box.vMax.y = box.vMin.y + height;
}

/**
 *	@brief vykresli herni postavu (pouzije se vhodna textura podle situace)
 */
void Hero::draw(){

	//kontrola inicializace
	try{	
		if(!isInit) throw "Error - Hero:: not initialized";		
	} catch (const char* msg){
		std::cerr << msg << std::endl;
		return;
	}

	//jednoducha animace pohybu
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

	if(jumping)
		glBindTexture(GL_TEXTURE_2D, hero_tex_jump);

	//vykresleni
	heroShader.bind(box.vMin, facingLeft);

	glBindVertexArray(heroVAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}

/**
 *	@brief cisteni
 */
void Hero::cleanupOpenGL(){
	heroShader.deleteShader();
}