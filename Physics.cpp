#include "Physics.h"

bool BoxDebug::standingOn = false;

/**
 *	@brief generovani geometrie a dalsi souvisejici inicializace
 *	@param[in] bmpfile textura herni urovne
 */
void BoxDebug::generate(TBmp * bmpfile){
	
	//najde kostky 1x1 v obrazku mapy
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


	//nastavit povrchy
	surfaceShader.compile("Shaders/surface_curve.vx.shader", "Shaders/surface_curve.fr.shader");
	surfaceSinus.compile("Shaders/surface_sinus.vx.shader", "Shaders/surface_sinus.fr.shader");
	surfaceZigzag.compile("Shaders/surface_zigzag.vx.shader", "Shaders/surface_zigzag.fr.shader");

	Surface sinusSurface(glm::vec3(0.7, 0.7, 0.7), 2.0f, 0.5f, surfaceSinus, 0.1f, false);
	Surface zigzagSurface(glm::vec3(0.0, 0.7, 0.7), 0.2f, 2.2f, surfaceZigzag, 0.1f, false);
	Surface digitalSurface(glm::vec3(0.5, 0.8, 0.5), 2.0f, 0.5, surfaceShader, 0.2f, false);

	Surface killSurface(glm::vec3(1.0, 0.1, 0.1), 0.2f, 2.2f, surfaceZigzag, 0.2f, true);

	for(unsigned int i = 0; i < boxes.size(); i++){
		Box a = boxes.at(i);

		if(i % 3 == 0)
			a.surface = digitalSurface;
		else if(i % 2 == 1)
			a.surface = zigzagSurface;
		else
			a.surface = sinusSurface;

		if(a.vMin.x == 0 && a.vMax.x == 1)
			a.surface = killSurface;

		boxes.at(i) = a;
	}

	phase = 0.0f;

	
	for(unsigned int i = 0; i < boxes.size(); i++){	
		Box a = boxes.at(i);
		std::cout << i << ". " << a << std::endl;
	}

	//pripravit shadery
	boxesShader.compile();
	pointSimpleShader.compile();

	//pripravit hmatovy vjem
	int numOfPoints = 50;
	float numOfPointsF = 50.0f;
	for(int i = 0; i < numOfPoints; i++){
		float points[] = {0.0f + i/numOfPointsF, 0.0f, 0.0f, 0.1f + i/numOfPointsF, 0.0f, 0.0f};
		pozice1D.insert(pozice1D.end(), points, points+12);
	}

	glGenBuffers(1, &tactileVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tactileVBO);
	glBufferData(GL_ARRAY_BUFFER, pozice1D.size() * sizeof(float), &pozice1D[0], GL_STATIC_DRAW);

	glGenVertexArrays(1, &tactileVAO);
	glBindVertexArray(tactileVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, tactileVBO);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, p_OffsetInVBO(0));
	}
	glBindVertexArray(0);
	
}

/**
 *	@brief nastavi smerovani vektor podle stisknutych klaves
 *	@param[in] k stisknute klavesy
 */
void BoxDebug::setMovementDirection(Keys k){

	if(k.up)
		movementDirection.y = -1.0;
	if(k.down)
		movementDirection.y = 1.0;
	if(k.right)
		movementDirection.x = 1.0;
	if(k.left)
		movementDirection.x = -1.0;

	//nebrat v potaz drzeni protichudnych klaves
	if(k.up && k.down)
		movementDirection.y = 0.0;
	if((k.left && k.right) || (!k.left && !k.right))
		movementDirection.x = 0.0;
}

/**
 *	@brief zpocita a aplikuje posun herni postavy podle casu snimku
 *	@param[in] frame_time cas snimku
 *	@param[in] key stisknute klavesy
 *	@param[in] hero herni postava
 */
void BoxDebug::calculateTimeBasedMovement(DWORD frame_time, Keys key, Hero * hero)
{
	setMovementDirection(key);

	if(movementDirection.x != 0.0)
		hero->facingLeft = (movementDirection.x == -1.0) ? true : false;

	gravity = DEFAULT_GRAVITY;

	//vyskok
	if(hero->jumping == false && key.up){
		hero->velocity.y = DEFAULT_JUMP_VELOCITY;
		hero->jumping = true;
	}

	//padani
	if(hero->jumping || !standingOn){
		hero->position.y += hero->velocity.y;
		hero->velocity.y += gravity;

		if(hero->velocity.y > 0.8f)//rychleji uz nedetekuje kolize se zemi
			hero->velocity.y = 0.8f;
	}

	//chuze
	if(hero->moving == false && movementDirection.x != 0.0){
		hero->moving = true;
		hero->velocity.x = DEFAULT_WALK_VELOCITY;
	}
	else
		hero->moving = false;

	//zastaveni chuze
	if((key.left && key.right) || (!key.left && !key.right))
		hero->velocity.x = 0.0;

	//pohyb podle casu snimku
	if(hero->moving){
		hero->position.x += (/*frame_time * 0.1f * */hero->velocity.x * movementDirection.x);
	}
}

/**
 *	@brief implementuje separating axis theorem - detekci kolizi
 *	@param[in] hero herni postava
 */
void BoxDebug::SeparatingAxisTheorem(Hero * hero){

	Box a = hero->box;
	standingOn = false;
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
		bool verticOver = false;
		bool horizLeft = false;

		//mira protnuti
		float verticInter, horizInter;	
		
		//rozsirene podminky kvuli ladeni
		if(aprojx.y < bprojx.x){
			//std::cout << "A B" << std::endl;
		}else if(bprojx.y < aprojx.x){
			//std::cout << "B A" << std::endl;
		}else{
			//std::cout << "horizontalni kolize" << std::endl;
			horizCol = true;

			if(aprojx.x > bprojx.x && aprojx.x <= bprojx.y){//B A
				horizInter = bprojx.y - aprojx.x;
			}else if(aprojx.y < bprojx.y && aprojx.y >= bprojx.x){//A B
				horizInter = aprojx.y - bprojx.x;
				horizLeft = true;
			}else{
				//std::cout << "kostka uvnitr HORIZONTAL" << std::endl;
				horizInter = 1000.0f;//musime se pohnout v Y ose
			}
		}
		
		//rozsirene podminky kvuli ladeni
		if(aprojy.y < bprojy.x){
			//std::cout << "A" << std::endl;
			//std::cout << "B" << std::endl;
		}
		else if(bprojy.y < aprojy.x){
			//std::cout << "B" << std::endl;
			//std::cout << "A" << std::endl;
		}
		else{
			//std::cout << "vertikalni kolize" << std::endl;
			verticCol = true;

			if(aprojy.y >= bprojy.x && aprojy.y < bprojy.y){//A nad B
				verticInter = aprojy.y - bprojy.x;
				verticOver = true;
			}else if(aprojy.x <= bprojy.y && aprojy.x > bprojy.x){//B nad A
				verticInter = bprojy.y - aprojy.x;
			}else{
				//std::cout << "kostka uvnitr VERTICAL" << std::endl;
				verticInter = 1000.0f;//musime se pohnout v X ose
			}
		}

		if(horizCol && verticCol){//doslo ke kolizi

			float gap = 0.0f;	

			//krajni body kolize
			float mostLeftX;
			float mostRightX;
			float mostTopY;
			float mostBottomY;
			collisionType type;

			if(verticInter < horizInter){//protnuti ve vertikalni ose je mensi

				//vyber nejlevejsiho a nejpravejsiho mista kolize
				mostLeftX = (a.vMin.x < b.vMin.x) ? b.vMin.x : a.vMin.x;
				mostRightX = (a.vMax.x > b.vMax.x) ? b.vMax.x : a.vMax.x;

				if(verticOver){//hrac stoji na prekazce
					//hero->position.y = b.vMin.y - (a.vMax.y - a.vMin.y) - gap;
					hero->jumping = false;
					standingOn = true;
					hero->position.y -= verticInter;

					mostTopY = bprojy.x;
					mostBottomY = bprojy.x;
					type = AoverB;

				}else{
					//hero->position.y = b.vMax.y + gap;
					hero->position.y += verticInter;

					mostTopY = bprojy.y;
					mostBottomY = bprojy.y;
					type = AunderB;
				}
			}else{

				//vyber nejvyssiho a nejnizsiho mista kolize
				mostTopY = (a.vMin.y < b.vMin.y) ? b.vMin.y : a.vMin.y;
				mostBottomY = (a.vMax.y > b.vMax.y) ? b.vMax.y : a.vMax.y;

				if(horizLeft){//hrac stoji nalevo od prekazky
					//hero->position.x = b.vMin.x - (a.vMax.x - a.vMin.x) - gap;
					hero->position.x -= horizInter;

					mostLeftX = bprojx.x;
					mostRightX = bprojx.x;
					type = AB;
				}else{
					//hero->position.x = b.vMax.x + gap;
					hero->position.x += horizInter;

					mostLeftX = bprojx.y;
					mostRightX = bprojx.y;
					type = BA;
				}
			}
		
			//pridani instance kolize do vektoru kolizi
			glm::vec2 point1(mostLeftX, mostTopY);
			glm::vec2 point2(mostRightX, mostBottomY);
			collisions.push_back(Collision(type, point1, point2, b.surface));

		}//horizCol && verticCol
	}//for boxes
}

/**
 *	@brief vykresli krajni body kolize - testovani
 */
void BoxDebug::drawCollisionsPoints(){
	//krajni body kolize
	if(collisions.size() > 0){

		for(unsigned int i = 0; i < collisions.size(); i++){
			Collision a = collisions.at(i);

			//std::cout << i << ". type: " << a.getTypeString() << " p1: " << a.point1.x << ", " << a.point1.y << " p2: " << a.point2.x << ", " << a.point2.y << std::endl;

			float bod[] = {a.point1.x,  a.point1.y, a.point2.x, a.point2.y};

			GLuint testVBO, testVAO;
			glGenBuffers(1, &testVBO);
			glBindBuffer(GL_ARRAY_BUFFER, testVBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(bod), bod, GL_STATIC_DRAW);

			glGenVertexArrays(1, &testVAO);
			glBindVertexArray(testVAO);
			{
				glBindBuffer(GL_ARRAY_BUFFER, testVBO);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, p_OffsetInVBO(0));
			}
			glBindVertexArray(0);

			pointSimpleShader.bind();
			glBindVertexArray(testVAO);
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);//vstup vystup barva
			glDrawArrays(GL_POINTS, 0, 2);
			glDisable(GL_BLEND);
			glBindVertexArray(0);

		}
	}
}

/**
 *	@brief vykresli hmatovy vjem (tedy vhodne kolize)
 *  maze vektor kolizi !!!
 */
void BoxDebug::drawCollisions(){

	//hmatovy vjem
	if(collisions.size() > 0){

		for(unsigned int i = 0; i < collisions.size(); i++){
			Collision a = collisions.at(i);

			//std::cout << a << std::endl;

			if(a.type == AoverB){//herni postava stoji na prekazce

				//posun faze cary hmatoveho vjemu podle teploty
				phase += a.surface.temperature;
				if(phase == 1.0f)
					phase = 0.0f;

				a.surface.surfaceShader.bind(a.point1, a.point2, a.surface.amplitude, a.surface.frequency, phase, a.surface.color);

				glBindVertexArray(tactileVAO);
				glDrawArrays(GL_LINES, 0, pozice1D.size() * 2);
				glBindVertexArray(0);
			}
		}
	}

	collisions.resize(0);
}

/**
 *	@brief zkontroluje kolize
 */
bool BoxDebug::checkCollisions(){

	if(collisions.size() > 0){

		for(unsigned int i = 0; i < collisions.size(); i++){
			Collision a = collisions.at(i);

			if(a.surface.kills){
				std::cout << "SMRT" << std::endl;
				std::cout << a << std::endl;

				return true;
			}
		}
	}

	return false;
}

/**
 *	@brief vykresli jinou barvou kostky, ktere zabiji - testovani
 */
void BoxDebug::drawKills(){

	if(boxes.size() > 0){

		glm::vec3 killBoxColor(0.5, 0.5, 0.5);
		for(unsigned int i = 0; i < boxes.size(); i++){

			Box a = boxes.at(i);

			if(a.surface.kills)
				drawBox(a, killBoxColor);
		}
	}
}

/**
 *	@brief vykresli geometrii dane kostky - testovani
 *	@param[in] a instance Box
  *	@param[in] color barva
 */
void BoxDebug::drawBox(Box a, glm::vec3 color){

	float a_vertices[] = {a.vMin.x, a.vMin.y, 1, /**/a.vMax.x , a.vMin.y, 1,/**/a.vMax.x, a.vMax.y, 1,/**/a.vMin.x, a.vMax.y, 1};
	int a_indices[] = {0, 1, 1, 2, 2, 3, 3, 0, 0, 2, 1, 3};
		
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

	boxesShader.bind(color);

	glBindVertexArray(VAO);
	glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	glBindVertexArray(0);
}

/**
 *	@brief vykresli geometrii herni urovne
 */
void BoxDebug::draw(){

	if(boxes.size() > 0){

		std::vector<float> p_box_vertices;
		std::vector<int> p_box_indices;

		//tvorba vektoru vrcholu a indicii
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

		glm::vec3 color(1.0, 0.3, 0.6);
		boxesShader.bind(color);
		glBindVertexArray(VAO);

		glDrawElements(GL_LINES, boxes.size()*12, GL_UNSIGNED_INT, p_OffsetInVBO(0));

		glBindVertexArray(0);
	}
}

/**
 *	@brief cisteni
 */
void BoxDebug::cleanupOpenGL(){
	boxesShader.deleteShader();
	pointSimpleShader.deleteShader();
	surfaceShader.deleteShader();
	surfaceSinus.deleteShader();
	surfaceZigzag.deleteShader();
}