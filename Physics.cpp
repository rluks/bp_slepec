#include "Physics.h"


void BoxDebug::generate(TBmp * bmpfile){

	/*
	for(int i = 0; i < bmpfile->n_width*bmpfile->n_height; i++){
		unsigned char pixel = bmpfile->p_buffer[i];
		char out = (pixel == 0xFF) ? ' ' : '*'; 
		printf("|%d|", pixel);
	}*/
		
	boxes.resize(0);
	std::string line("+");
	for(int j = 0; j < bmpfile->n_height; j++){
		for(int i = 0; i < bmpfile->n_width; i++){
			unsigned char pixel = bmpfile->p_buffer[i + bmpfile->n_width * j];
			char out = (pixel == 0x00) ? ' ' : '*'; 
			if(pixel == 0x00){//prekazka
				Box box;
				box.vMin = glm::vec2(i, j);
				box.vMax = glm::vec2(i+1, j+1);
				boxes.push_back(box);
			}
			line+= out;
		}
		std::cout << "|" << line << "|" << std::endl;
		line.clear();
	}

	//rucne vytvorena testovaci data
	/*
	Box a;
	Box b;
	a.vMin = glm::vec2(0.0, 0.0);
	a.vMax = glm::vec2(1.0, 1.0);
	b.vMin = glm::vec2(5.0, 3.0);
	b.vMax = glm::vec2(7.0, 6.0);

	boxes.push_back(a);
	boxes.push_back(b);*/

	/*
	for(unsigned int i = 0; i < boxes.size(); i++){
		Box a = boxes.at(i);
		std::cout << "box no." << i << ":" << "vMinx: " << a.vMin.x << " vMiny: " << a.vMin.y << "vMaxx: " << a.vMax.x << " vMaxy: " << a.vMax.y << std::endl;
	}*/

	//pripravit shader
	boxesShader.compile();
	
}

void BoxDebug::draw(){

	if(boxes.size() > 0){
		std::vector<float> p_box_vertices;
		std::vector<int> p_box_indices;
		for(unsigned int i = 0; i < boxes.size(); i++){
			Box a = boxes.at(i);
			float a_vertices[] = {a.vMin.x, a.vMin.y, 1, /**/a.vMax.x , a.vMin.y, 1,/**/a.vMax.x, a.vMax.y, 1,/**/a.vMin.x, a.vMax.y, 1};
			int a_indices[] = {0+i*4, 1+i*4, 1+i*4, 2+i*4, 2+i*4, 3+i*4, 3+i*4, 0+i*4, 0+i*4, 2+i*4, 1+i*4, 3+i*4};
			p_box_vertices.insert(p_box_vertices.end(), a_vertices, a_vertices+12);
			p_box_indices.insert(p_box_indices.end(), a_indices, a_indices+12);
		}
		
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, p_box_vertices.size() * sizeof(float), &p_box_vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_box_indices.size() * sizeof(int), &p_box_indices[0], GL_STATIC_DRAW);

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, p_OffsetInVBO(0));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		}
		glBindVertexArray(0);

		boxesShader.bind();

		glBindVertexArray(VAO);
		glDrawElements(GL_LINES, boxes.size()*12, GL_UNSIGNED_INT, p_OffsetInVBO(0));
		glBindVertexArray(0);
	}
}