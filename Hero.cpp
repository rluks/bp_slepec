#include "Hero.h"

void Hero::init(){

	heroShader.compile();

	TBmp *p_box;
	if(!(p_box = CTgaCodec::p_Load_TGA("player1.tga"))) {
		fprintf(stderr, "error: failed to load \'.tga file\'\n");
		//return false;
	}

	hero_tex = n_GLTextureFromBitmap(p_box, GL_RGBA8);
	p_box->Delete();
}

void Hero::draw(glm::vec2 movement){


		//if(c) throw "Hero not initialized";

	const float p_hero_vertices[] = {
	0, 0, 0, 0, 1, /**/1, 0, width, 0, 1,/**/ 1, 1, width, height, 1,/**/ 0, 1, 0, height, 1};
	// vrcholy krychle, vzdy 2 souradnice textury, pak 3 souradnice vrcholu; vrcholy jsou usporadane poporade po ctvericich, tvoricich polygony jednotlivych sten

	const unsigned int p_hero_indices[] = {	
		0, 1, 2, 0, 3, 2};
		
	GLuint heroVBO, heroVAO, heroIBO;
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


	glBindTexture(GL_TEXTURE_2D, hero_tex);

	heroShader.bind(movement);

	glBindVertexArray(heroVAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	glDisable(GL_BLEND);
	glBindVertexArray(0);
}