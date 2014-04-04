#include "Physics.h"


void BoxDebug::generate(/*std::vector<Box> &boxes, */BMPFile * bmpfile){

	std::cout << "w: " << bmpfile->width << " h: " << bmpfile->height << " s: " << bmpfile->imageSize << std::endl;

	/*
	for(int i = 0; i < bmpfile->width*bmpfile->height; i++){
		unsigned char pixel = bmpfile->data[i];
		char out = (pixel == 0xFF) ? ' ' : '*'; 
		printf("|%d|", pixel);
	}*/

	/*
	for(int i = 0; i < bmpfile->width; i++){
		std::string line;
		for(int j = 0; j < bmpfile->height; j++){
			unsigned char pixel = bmpfile->data[i + bmpfile->width * j];
			char out = (pixel == 0xFF) ? ' ' : '*'; 
			//std::cout << std::hex << pixel << std::endl;
			//printf("|%d|", pixel);
			line+=out;
		}
		std::cout << "|" << line << "|" << std::endl;
		line.clear();
	}*/

	//rucne vytvorena testovaci data
	Box a;
	Box b;
	a.vMin = glm::vec2(0.0, 0.0);
	a.vMax = glm::vec2(1.0, 1.0);
	b.vMin = glm::vec2(5.0, 3.0);
	b.vMax = glm::vec2(7.0, 6.0);

	boxes.push_back(a);
	boxes.push_back(b);


	//pripravit shader
	boxesShader.compile();
	
}

void BoxDebug::draw(){
	Box a = boxes.at(1);
	const float p_box_vertices[] = {
		0, 0, a.vMin.x, a.vMin.y, 1, /**/1, 0, a.vMax.x , a.vMin.y, 1,/**/ 1, 1, a.vMax.x, a.vMax.y, 1,/**/ 0, 1, a.vMin.x, a.vMax.y, 1};
	// vrcholy krychle, vzdy 2 souradnice textury, pak 3 souradnice vrcholu; vrcholy jsou usporadane poporade po ctvericich, tvoricich polygony jednotlivych sten

	const unsigned int p_box_indices[] = {	
		0, 1, 1, 2, 2, 3, 3, 0, 0, 2, 1, 3};
		
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_box_vertices), p_box_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_box_indices), p_box_indices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), p_OffsetInVBO(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), p_OffsetInVBO(2 * sizeof(float)));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	}
	glBindVertexArray(0);

	boxesShader.bind();

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	glBindVertexArray(0);
}