#include "Physics.h"

void BoxDebug::generate(TBmp * bmpfile){

	/*
	for(int i = 0; i < bmpfile->n_width*bmpfile->n_height; i++){
		unsigned char pixel = bmpfile->p_buffer[i];
		char out = (pixel == 0xFF) ? ' ' : '*'; 
		printf("|%d|", pixel);
	}*/
		
	boxes.resize(0);
	//std::cout << "prvni pixel +, 0x00 *, 0xFF mezera" << std::endl;
	std::string line("+");
	for(int j = 0; j < bmpfile->n_height; j++){
		for(int i = 0; i < bmpfile->n_width; i++){
			unsigned char pixel = bmpfile->p_buffer[i + bmpfile->n_width * j];
			char out = (pixel == 0x00) ? '*' : ' '; 
			if(pixel == 0x00){//prekazka
				Box box;
				box.vMin = glm::vec2(i, j);
				box.vMax = glm::vec2(i+1, j+1);
				boxes.push_back(box);
			}
			line+= out;
		}
		//std::cout << "|" << line << "|" << std::endl;
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

	//sloucit kostky po 2
	/*
	for(unsigned int i = 0; i < boxes.size(); i++){	
		if(i+1 != boxes.size()){
			Box a = boxes.at(i);
			Box b = boxes.at(i+1);
			std::cout << "box no." << i << ":" << "vMinx: " << a.vMin.x << " vMiny: " << a.vMin.y << "vMaxx: " << a.vMax.x << " vMaxy: " << a.vMax.y << std::endl;
			std::cout << "box no." << i+1 << ":" << "vMinx: " << b.vMin.x << " vMiny: " << b.vMin.y << "vMaxx: " << b.vMax.x << " vMaxy: " << b.vMax.y << std::endl;
			if((a.vMin.y == b.vMin.y) && (a.vMin.x+1 == b.vMin.x)){
				Box merge;
				merge.vMin.x = a.vMin.x;
				merge.vMin.y = a.vMin.y;
				merge.vMax.x = b.vMax.x;
				merge.vMax.y = b.vMax.y;
				boxes.at(i) = merge;
				boxes.erase(boxes.begin() + i+1);
			}
		}
	}*/

	//najit vhodne kostky ke slouceni na delku
	for(unsigned int i = 0; i < boxes.size(); i++){	
		Box a = boxes.at(i);
		int boxwidth = (int) (a.vMax.x - a.vMin.x);
		//std::cout << "a box no." << i << ":" << "vMinx: " << a.vMin.x << " vMiny: " << a.vMin.y << "vMaxx: " << a.vMax.x << " vMaxy: " << a.vMax.y << " width: " << boxwidth << std::endl;
			for(unsigned int j = i+1; j < boxes.size(); j++){	
				Box b = boxes.at(j);
				int boxwidthb = (int) (b.vMax.x - b.vMin.x);
				if((a.vMin.y == b.vMin.y && a.vMin.x+boxwidth == b.vMin.x) && (a.vMax.y == b.vMax.y && a.vMax.x+boxwidthb == b.vMax.x)){
					//std::cout << "b box no." << j << ":" << "vMinx: " << b.vMin.x << " vMiny: " << b.vMin.y << "vMaxx: " << b.vMax.x << " vMaxy: " << b.vMax.y << " width: " << boxwidthb << std::endl;
					a.vMax = b.vMax;
					boxwidth = (int) (a.vMax.x - a.vMin.x);
					boxes.erase(boxes.begin() + j);
					j--; //!!! menime delku vektoru, takze oskliva zmena indexu uvnitr bloku for cyklu
				}

			}
			boxes.at(i) = a;//ulozeni vysledne zmenene kostky (resp. obdelniku)
	}

	//najit vhodne kostky ke slouceni na vysku
	for(unsigned int i = 0; i < boxes.size(); i++){	
		Box a = boxes.at(i);
		int boxwidth = (int) (a.vMax.x - a.vMin.x);
		int boxheight = (int) (a.vMax.y - a.vMin.y);
		//std::cout << "a box no." << i << ":" << "vMinx: " << a.vMin.x << " vMiny: " << a.vMin.y << "vMaxx: " << a.vMax.x << " vMaxy: " << a.vMax.y << " width: " << boxwidth << " height: " << boxheight << std::endl;
			for(unsigned int j = i+1; j < boxes.size(); j++){	
				Box b = boxes.at(j);
				int boxwidthb = (int) (b.vMax.x - b.vMin.x);
				int boxheightb = (int) (b.vMax.y - b.vMin.y);

				if((a.vMin.x == b.vMin.x && a.vMin.y+boxheight == b.vMin.y) && (a.vMax.x == b.vMax.x && a.vMax.y+boxheightb == b.vMax.y)){
					//std::cout << "b box no." << j << ":" << "vMinx: " << b.vMin.x << " vMiny: " << b.vMin.y << "vMaxx: " << b.vMax.x << " vMaxy: " << b.vMax.y << " width: " << boxwidthb << " height: " << boxheightb << std::endl;
					a.vMax = b.vMax;
					boxwidth = (int) (a.vMax.x - a.vMin.x);
					boxheight = (int) (a.vMax.y - a.vMin.y);
					boxes.erase(boxes.begin() + j);
					j--; //!!! menime delku vektoru, takze oskliva zmena indexu uvnitr bloku for cyklu
				}

			}
			boxes.at(i) = a;//ulozeni vysledne zmenene kostky (resp. obdelniku)
	}

	//pripravit shader
	boxesShader.compile();
	
}

void BoxDebug::setMovementDirection(Keys k){

	if(k.up)
		movementDirection.y = -1.0;
	if(k.down)
		movementDirection.y = 1.0;
	if(k.right)
		movementDirection.x = 1.0;
	if(k.left)
		movementDirection.x = -1.0;

	if(k.up && k.down)
		movementDirection.y = 0.0;
	if((k.left && k.right) || (!k.left && !k.right))
		movementDirection.x = 0.0;
}

void BoxDebug::calculateTimeBasedMovement(DWORD frame_time, Keys key, Hero * hero)
{
	setMovementDirection(key);

	std::cout << "y:" << hero->position.y << std::endl;

	////
	float gravity = 0.03f;
	if(hero->jumping == false && key.space){
		hero->velocity.y = -0.5;
		hero->jumping = true;
	}

	if(hero->jumping){
		if(!verticOver){
			hero->position.y += hero->velocity.y;
			hero->velocity.y += gravity;
		}else{
			hero->jumping = false;
		}
	}

	if(hero->moving == false && movementDirection.x != 0.0){
		hero->moving = true;
		hero->velocity.x = 0.1f;
	}
	else
		hero->moving = false;
	////

	if(hero->moving)
		hero->position.x += (/*frame_time * 0.1f * */hero->velocity.x * movementDirection.x);


	//position->x += frame_time * 0.25f * movementDirection.x * 0.01f;
	//position->y += frame_time * 0.5f * movementDirection.y * 0.01f;


	//position += velocity * dt;
    //velocity += acceleration * dt;

	/*
	if(k.up)
		position.y += frame_time * 0.5f * movementDirection.y * 0.01f;
	else{
		//float gravity = (boxDebug.verticOver) ? 0.0 : 0.0000005f;
		float gravity = 0.0000005f;

		position.y += frame_time * (velocityVec.y + frame_time * gravity/2.0f);
		velocityVec.y += frame_time * gravity;
	}

	std::cout << "velocityVec.y: " << velocityVec.y << std::endl;*/

	
}

void BoxDebug::SeparatingAxisTheorem(Box a, glm::vec2 * position){

	for(unsigned int i = 0; i < boxes.size(); i++){

		Box b = boxes.at(i);

		//projekce na y
		glm::vec2 aprojy(a.vMin.y, a.vMax.y);
		glm::vec2 bprojy(b.vMin.y, b.vMax.y);

		//projekce na x
		glm::vec2 aprojx(a.vMin.x, a.vMax.x);
		glm::vec2 bprojx(b.vMin.x, b.vMax.x);

		//kolize?
		bool verticCol = false;
		bool horizCol = false;

		//smer
		verticOver = false;
		bool horizLeft = false;

		//mira protnuti
		float verticInter;
		float horizInter;
		
		/*
		if(aprojx.y >= bprojx.x && bprojx.y >= aprojx.x){
			horizCol = true;

			if(aprojx.x >= bprojx.x && aprojx.x <= bprojx.y){//B A
				//std::cout << "B A" << std::endl;
				horizInter = bprojx.y - aprojx.x;
			}else if(aprojx.y <= bprojx.y && aprojx.y >= bprojx.x){//A B
				//std::cout << "A B" << std::endl;
				horizInter = aprojx.y - bprojx.y;
				horizLeft = true;
			}else{
				std::cout << "NEZNAM HORIZONTAL" << std::endl;//uvnitr
				horizInter = 0.0;
			}
		}*/
		
		
		if(aprojx.y < bprojx.x){
			//std::cout << "A B" << std::endl;
		}else if(bprojx.y < aprojx.x){
			//std::cout << "B A" << std::endl;
		}else{
			//std::cout << "ine" << std::endl;
			horizCol = true;

			if(aprojx.x >= bprojx.x && aprojx.x <= bprojx.y){//B A
				//std::cout << "B A" << std::endl;
				horizInter = bprojx.y - aprojx.x;
			}else if(aprojx.y <= bprojx.y && aprojx.y >= bprojx.x){//A B
				//std::cout << "A B" << std::endl;
				horizInter = aprojx.y - bprojx.y;
				horizLeft = true;
			}else{
				//std::cout << "NEZNAM HORIZONTAL" << std::endl;//kostka uvnitr
				horizInter = 0.0;
			}
		}
		
		if(aprojy.y >= bprojy.x && bprojy.y >= aprojy.x){
			verticCol = true;

			if(aprojy.y >= bprojy.x && aprojy.y <= bprojy.y){//A nad B
				verticInter = aprojy.x - bprojy.y;
				verticOver = true;
			}else if(bprojy.y >= aprojy.x && bprojy.y <= aprojy.y){//B nad A
				verticInter = bprojy.x - aprojy.y;
			}else{
				std::cout << "NEZNAM VERTICAL" << std::endl;
				verticInter = 0.0;
			}
		}

		/*
		if(aprojy.y < bprojy.x){
			//std::cout << "A" << std::endl;
			//std::cout << "B" << std::endl;
		}
		else if(bprojy.y < aprojy.x){
			//std::cout << "B" << std::endl;
			//std::cout << "A" << std::endl;
		}
		else{
			//std::cout << "ine" << std::endl;
			verticCol = true;
		}*/

		if(horizCol && verticCol){
			//std::cout << "position.x: " << position->x << " position.y: " << position->y << std::endl;
			//std::cout << "hrac(Min(" << a.vMin.x << ", " << a.vMin.y <<"), Max(" << a.vMax.x <<", " << a.vMax.y << ")) s " << "Min(" << b.vMin.x << ", " << b.vMin.y <<"), Max(" << b.vMax.x <<", " << b.vMax.y << ")" << std::endl;

			//upravit position, aby nedoslo ke kolizi
			float gap = 0.1f;
			if(verticInter < horizInter){
				if(verticOver){
					position->y = b.vMin.y - (a.vMax.y - a.vMin.y) - gap;
				}else
					position->y = b.vMax.y + gap;
			}else{
				if(horizLeft)
					position->x = b.vMin.x - (a.vMax.x - a.vMin.x) - gap;
				else
					position->x = b.vMax.x + gap;
			}
		}
	}
}


void BoxDebug::drawBox(Box a){

	float a_vertices[] = {a.vMin.x, a.vMin.y, 1, /**/a.vMax.x , a.vMin.y, 1,/**/a.vMax.x, a.vMax.y, 1,/**/a.vMin.x, a.vMax.y, 1};
	int a_indices[] = {0, 1, 1, 2, 2, 3, 3, 0, 0, 2, 1, 3};

	//std::cout << "drawBox: vMin: " << a.vMin.x << ", " << a.vMin.y << std::endl;
		
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(a_vertices), a_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(a_indices), a_indices, GL_STATIC_DRAW);

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
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	glBindVertexArray(0);
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