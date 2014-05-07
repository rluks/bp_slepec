#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <istream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

#include <glm/vec2.hpp> 

#include <ft2build.h>
#include FT_FREETYPE_H

#include "OpenGL30Drv.h"
#include "Transform.h"
#include "Tga.h"
#include "Shader.h"
#include "Texture.h"
#include "Common.h"
#include "Physics.h"
#include "Hero.h"
#include "Level.h"

static const char *p_s_window_name = "Slepec";
static const char *p_s_class_name = "my_wndclass";
// jmeno okna

static int n_width = 640;
static int n_height = 480;
// velikost okna

static bool b_running = true;
// flag pro vyskoceni ze smycky zprav

void OnIdle(CGL30Driver &driver);
void onResize(int n_x, int n_y, int n_new_width, int n_new_height);
LRESULT CALLBACK WndProc(HWND h_wnd, UINT n_msg, WPARAM n_w_param, LPARAM n_l_param);
void drawPingPong(int);
void drawDrop(bool, float, float);
void drawDrop2(bool, float, float, int);
void clearTexture(int);
// prototypy funkci

GLuint fbo;
int n_source_tex;
static const GLenum p_draw_buffers[] = {GL_COLOR_ATTACHMENT0};
static GLuint n_vertex_buffer_object, n_index_buffer_object, n_vertex_array_object;

//textury
static GLuint n_texture[3][2];//r-height,g-y velocity,b-x velocity
static GLuint depthmap;

//shadery
ShaderBlur blurShader;
ShaderPass passShader;
ShaderVelocity velocityShader;
ShaderHeight heightShader;
ShaderPoint pointShader;

static DWORD frame_time;
Keys key = {false, false, false, false, false, false};



std::vector<POINTS> pointsQueue;

//hero
Hero hero;

//fyika
BoxDebug boxDebug;
static int cnt = 0;

std::vector<Level> levels;
unsigned int currentLevelIndex;

void clearTexture(int textnum){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_texture[textnum][0], 0);

	glViewport(0, 0, datawidth, dataheight);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glClear ( GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, n_texture[textnum][0]);


	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

}



void drawDrop2(bool clear, float x, float y, int textnum){

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_texture[textnum][0], 0);

	glViewport(0, 0, datawidth, dataheight);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	if(clear)
		glClear ( GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, n_texture[textnum][0]);

	float s = (2.0f * x)/32.0f - 1.0f;
	float t = 1.0f - (2.0f * y)/24.0f;
	float bod[] = {s, t};

	//std::cout << "kapka hulkou na souradnicich - s(x): " << s << " |t(y): " << t << std::endl;

	GLuint bodVBO, bodVAO;
	glGenBuffers(1, &bodVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bodVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bod), bod, GL_STATIC_DRAW);

	glGenVertexArrays(1, &bodVAO);
	glBindVertexArray(bodVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, bodVBO);
		glEnableVertexAttribArray(1);//TODO nastavujem rucne na 1, nula compile shader upravit
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, p_OffsetInVBO(0));
	}
	glBindVertexArray(0);

	pointShader.bind();
	glBindVertexArray(bodVAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);//vstup vystup barva

	glDrawArrays(GL_POINTS, 0, 1);

	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void drawDrop(bool clear, float x, float y){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_texture[0][0], 0);
	glViewport(0, 0, datawidth, dataheight);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	if(clear)
		glClear ( GL_COLOR_BUFFER_BIT);
	
	glBindTexture(GL_TEXTURE_2D, n_texture[0][0]);

	float bod[] = {x, y};

	GLuint bodVBO, bodVAO;
	glGenBuffers(1, &bodVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bodVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bod), bod, GL_STATIC_DRAW);

	glGenVertexArrays(1, &bodVAO);
	glBindVertexArray(bodVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, bodVBO);
		glEnableVertexAttribArray(1);//TODO nastavujem rucne na 1, nula compile shader upravit
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, p_OffsetInVBO(0));
	}
	glBindVertexArray(0);

	pointShader.bind();
	glBindVertexArray(bodVAO);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);//vstup vystup barva

	glDrawArrays(GL_POINTS, 0, 1);

	glDisable(GL_BLEND);
	glBindVertexArray(0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}



/**
 *	@brief vytvori vsechny OpenGL objekty, potrebne pro kresleni
 *	@return vraci true pri uspechu, false pri neuspechu
 */
bool InitGLObjects()
{

	const float p_box_vertices[] = {

	//0, 1, -1, -1, 1, /**/1, 1, 1, -1, 1,/**/1, 0, 1, 1, 1,/**/0, 0, -1, 1, 1};//textura spravne, simulace nefunguje
	0, 0, -1, -1, 1, /**/1, 0, 1, -1, 1,/**/ 1, 1, 1, 1, 1,/**/ 0, 1, -1, 1, 1};//puvodne - textura vzhuru nohama, simulace funguje
	// vrcholy krychle, vzdy 2 souradnice textury, pak 3 souradnice vrcholu; vrcholy jsou usporadane poporade po ctvericich, tvoricich polygony jednotlivych sten

	const unsigned int p_box_indices[] = {	
		0, 1, 2, 0, 2, 3};

	/*
	const float p_box_vertices[] = {
		0, 1, -1, -1, 1, 1, 1, 1, -1, 1, 1, 0, 1, 1, 1, 0, 0, -1, 1, 1,
		1, 1, -1, -1, -1, 1, 0, -1, 1, -1, 0, 0, 1, 1, -1, 0, 1, 1, -1, -1,
		0, 0, -1, 1, -1, 0, 1, -1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, -1,
		1, 0, -1, -1, -1, 0, 0, 1, -1, -1, 0, 1, 1, -1, 1, 1, 1, -1, -1, 1,
		1, 1, 1, -1, -1, 1, 0, 1, 1, -1, 0, 0, 1, 1, 1, 0, 1, 1, -1, 1,
		0, 1, -1, -1, -1, 1, 1, -1, -1, 1, 1, 0, -1, 1, 1, 0, 0, -1, 1, -1};
	// vrcholy krychle, vzdy 2 souradnice textury, pak 3 souradnice vrcholu; vrcholy jsou usporadane poporade po ctvericich, tvoricich polygony jednotlivych sten

	const unsigned int p_box_indices[] = {	//  0     1
		0, 1, 2, 0, 2, 3,					//  *-----*
		4, 5, 6, 4, 6, 7,					//  |\    |
		8, 9, 10, 8, 10, 11,				//  |  \  |
		12, 13, 14, 12, 14, 15,				//  |    \|
		16, 17, 18, 16, 18, 19,				//  *-----*
		20, 21, 22, 20, 22, 23};			//  3     2
	// indexy trojuhelniku krychle (kazda stena se sklada ze dvou trojuhelniku, viz. ascii art); v OpenGL 3.0 uz nemame GL_QUADS a tak musime takto kreslit trojuhelniky
	*/

	glGenBuffers(1, &n_vertex_buffer_object);
	glBindBuffer(GL_ARRAY_BUFFER, n_vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, sizeof(p_box_vertices), p_box_vertices, GL_STATIC_DRAW);
	// vyrobime buffer objekt, do ktereho zkopirujeme geometrii naseho modelu (do jednoho bufferu ulozime vsechny souradnice, tzn. texturovaci / normaly / barvy / pozice, atp. jeden buffer muze obsahovat data pro jeden nebo i vice modelu)

	glGenBuffers(1, &n_index_buffer_object);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, n_index_buffer_object);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(p_box_indices), p_box_indices, GL_STATIC_DRAW);
	// vytvorime druhy buffer objekt, do ktereho zkopirujeme indexy vrcholu naseho modelu (jedna se o jiny typ bufferu)

	glGenVertexArrays(1, &n_vertex_array_object);
	glBindVertexArray(n_vertex_array_object);
	{
		glBindBuffer(GL_ARRAY_BUFFER, n_vertex_buffer_object);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), p_OffsetInVBO(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), p_OffsetInVBO(2 * sizeof(float)));
		// rekneme OpenGL odkud si ma brat data; kazdy vrchol ma 5 souradnic,
		// souradnice textury maji offset 0, souradnice vrcholu maji offset 2

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, n_index_buffer_object);
		// rekneme OpenGL odkud bude brat indexy geometrie pro glDrawElements (nize)
	} // tento blok se "zapamatuje" ve VAO-vertex array objects
	glBindVertexArray(0); // vratime VAO 0, abychom si nahodne VAO nezmenili (pripadne osetreni proti chybe v ovladacich nvidia kde se VAO poskodi pri volani nekterych wgl funkci)
	// vytvorime a nastavime vertex array objekt - objekt na GPU, ktery si pamatuje konfiguraci nastaveni VBO. pak pri kresleni volame jen glBindVertexArray() namisto vsech prikazu v bloku vyse

	//shadery
	blurShader.compile();
	passShader.compile();
	velocityShader.compile();
	heightShader.compile();
	pointShader.compile();

	//urovne
	//0
	Box levelExit;
	levelExit.vMin.x = 28;
	levelExit.vMin.y = 20;
	levelExit.vMax.x = levelExit.vMin.x+1;
	levelExit.vMax.y = levelExit.vMin.y+1;
	Level levelZero(glm::vec2(3.5, 20), "Textures/0.png", levelExit);
	levelZero.addSoundSource(glm::vec2(4, 4));
	levels.push_back(levelZero);

	//1
	levelExit.vMin.x = 28;
	levelExit.vMin.y = 20;
	levelExit.vMax.x = levelExit.vMin.x+1;
	levelExit.vMax.y = levelExit.vMin.y+1;
	Level levelOne(glm::vec2(3.5, 10), "Textures/1.png", levelExit);
	//levelZero.addSoundSource(glm::vec2(levelExit.vMin.x+0.5, levelExit.vMin.y+0.5));
	levelOne.addSoundSource(glm::vec2(4, 4));
	levelOne.addSoundSource(glm::vec2(4, 8));
	levels.push_back(levelOne);

	hero.init();
	hero.position = levelZero.heroStartPosition;
	
	//textury
	for(unsigned int i = 0; i < 3; i++){
		n_texture[i][1] = n_GLTextureRGBFloat();
		n_texture[i][0] = n_GLTextureRGBFloat();
		std::cout << "i" << i << std::endl;
	}
	//n_texture[1][1] = n_GLTextureRGBFloat();
	//n_texture[1][0] = n_GLTextureRGBFloat();

	//fyzika
	TBmp * bitmap;
	if(!(bitmap = CPngCodec::p_Load_PNG(levelZero.mapFilepath.c_str()))){
		fprintf(stderr, "error: failed to load \'.png file\'\n");
		return false;
	}
	boxDebug.generate(bitmap);
	depthmap = n_GLTextureFromBitmap(bitmap, GL_RGBA8);
	bitmap->Delete();

	//FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glDrawBuffers(1, p_draw_buffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	n_source_tex = 0;

	//cisteni vln
	clearTexture(0);
	clearTexture(1);
	clearTexture(2);


	return true;
}

/**
 *	@brief uvolni vsechny OpenGL objekty
 */
void CleanupGLObjects()
{
	glDeleteBuffers(1, &n_vertex_buffer_object);
	glDeleteBuffers(1, &n_index_buffer_object);
	// smaze vertex buffer objekty

	glDeleteVertexArrays(1, &n_vertex_array_object);
	// smaze vertex array objekt

	hero.cleanupOpenGL();
	boxDebug.cleanupOpenGL();
	// smaze dalsi opengl objekty

	blurShader.deleteShader();
	passShader.deleteShader();
	velocityShader.deleteShader();
	heightShader.deleteShader();
	pointShader.deleteShader();
	// smaze shadery

	glDeleteTextures(4, n_texture[0]);
	glDeleteTextures(1, &depthmap);
	// smaze textury
}

/**
 *	@brief vykresli geometrii sceny (bez specifikace cehokoliv jineho nez geometrie)
 */
void DrawScene()
{
	glBindVertexArray(n_vertex_array_object);
	// rekne OpenGL odkud se berou data pro kresleni (z nasich VBO)

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	// nakresli objekt pomoci jedineho prikazu
		
	glDisable(GL_BLEND);

	glBindVertexArray(0); // vratime VAO 0, abychom si nahodne VAO nezmenili (pripadne osetreni proti chybe v ovladacich nvidia kde se VAO poskodi pri volani nekterych wgl funkci)
}

/**
 *	@brief main
 *	@param[in] n_arg_num je pocet argumentu prikazove radky
 *	@param[in] p_arg_list je seznam argumentu prikazove radky; tento program neni nejak ovladan prikazovou radkou a ignoruje ji
 *	@return vreci 0 pri uspesnem ukonceni programu, pripadne -1 pri chybe
 */
int main(int n_arg_num, const char **p_arg_list)
{
	WNDCLASSEX t_wnd_class;
	t_wnd_class.cbSize = sizeof(WNDCLASSEX);
	t_wnd_class.style = CS_HREDRAW | CS_VREDRAW;
	t_wnd_class.lpfnWndProc = WndProc;
	t_wnd_class.cbClsExtra = 0;
	t_wnd_class.cbWndExtra = 0;
	t_wnd_class.hInstance = GetModuleHandle(NULL);
	t_wnd_class.hIcon = LoadIcon(0, IDI_APPLICATION);
	t_wnd_class.hCursor = LoadCursor(0, IDC_ARROW);
	t_wnd_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	t_wnd_class.lpszMenuName = 0;
	t_wnd_class.lpszClassName = p_s_class_name;
	t_wnd_class.hIconSm = LoadIcon(0, IDI_WINLOGO);

	if(!RegisterClassEx(&t_wnd_class)){
		MessageBox(NULL, "Failed to register window class", NULL, MB_OK);
		return false;
	}
	// registruje tridu okna

	RECT t_rect = {0, 0, n_width, n_height};
	AdjustWindowRectEx(&t_rect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
	int n_wnd_width = t_rect.right - t_rect.left;
	int n_wnd_height = t_rect.bottom - t_rect.top;
	// spocita rozmery tak, aby vnitrek okna (client area) mel pozadovane rozmery

	HWND h_wnd;
	h_wnd = CreateWindow(p_s_class_name, p_s_window_name, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, n_wnd_width, n_wnd_height, NULL, NULL, GetModuleHandle(NULL), NULL);
	ShowWindow(h_wnd, SW_SHOW);
	UpdateWindow(h_wnd);
	// vyrobi a ukaze okno

	bool b_forward_compatible = true; // jestli ma byt OpenGL kontext dopredne kompatibilni (tzn. nepodporovat funkce "stareho" OpenGL); pokud vase GPU nepodporuje OpenGL 3.0 nebo vyssi, pak vam to s timto nepobezi
	CGL30Driver driver;
	if(!driver.Init(h_wnd, b_forward_compatible, 3, 0, n_width, n_height, 32, 24, 0, false)) {
		fprintf(stderr, "error: OpenGL initialization failed\n"); // nepodarilo se zinicializovat OpenGL
		return -1;
	}
	// inicializuje OpenGL

	printf("OpenGL initialized: %s\n", (const char*)glGetString(GL_VERSION));
	// vytiskne verzi OpenGL

	{
		int n_extension_num;
		glGetIntegerv(GL_NUM_EXTENSIONS, &n_extension_num);
		for(int i = 0; i < n_extension_num; ++ i) {
			const char *p_s_ext_name = (const char*)glGetStringi(GL_EXTENSIONS, i); // glGetString(GL_EXTENSIONS) uz v OpenGL 3 nefrci, protoze aplikace mely historicky problemy s prilis dlouhymi stringy. ted se extensions zjistuji po jedne.
			printf((i)? ", %s" : "%s", p_s_ext_name);
		}
		printf("\n");
	}
	if(!GL_VERSION_3_0) {
		fprintf(stderr, "error: OpenGL 3.0 not supported\n"); // OpenGL 3.0 neni podporovane
		return -1;
	}
	// zkontroluje zda jsou podporovane pozadovane rozsireni

	printf("\nGLSL version: %s\n", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	if(!InitGLObjects()) {
		fprintf(stderr, "error: failed to initialize OpenGL objects\n"); // neco se nepovedlo (chyba pri kompilaci shaderu / chyba pri nacitani obrazku textury)
		return -1;
	}
	// vyrobime objekty OpenGL, nutne ke kresleni

	FT_Library library;
	FT_Error error;
	error = FT_Init_FreeType( &library );
	if(error){
		std::cerr << "Problem with FreeType library." << std::endl;
	}

	//inicializace promennych
	srand (static_cast <unsigned> (time(0)));
	hero.velocity.x = 0.0;
	hero.velocity.y = 0.0;
	currentLevelIndex = 0;

	DWORD start_time =  GetTickCount(); // cas pro animaci
	DWORD end_time;
	frame_time = 15; //0.015 tj cca 1/60

	MSG t_msg;
	while(b_running) {
		if(PeekMessage(&t_msg, h_wnd, 0, 0, PM_REMOVE)) {
			TranslateMessage(&t_msg);
			DispatchMessage(&t_msg);
		} else {
			OnIdle(driver);
#ifdef _DEBUG
			if(glGetError() != GL_NO_ERROR)
				fprintf(stderr, "error: OpenGL error(s) occured\n");
#endif //_DEBUG
		}

		end_time = GetTickCount(); // cas pro animaci
		frame_time = end_time - start_time;
		//std::cout << "delta t: " << frame_time << std::endl;//0.015 tj cca 1/60
		start_time = end_time;
	}
	// smycka zprav, s funkci OnIdle()

	std::cout << "clean up..." << std::endl;
	CleanupGLObjects();
	// uvolnime OpenGL objekty

	std::cout << "...finished" << std::endl;
	//system("PAUSE");
	return 0;
}

/**
 *	@brief obsluha smycky zprav
 *	@param[in] h_wnd je okno pro ktere je zprava urcena
 *	@param[in] n_msg je cislo zpravy (WM_*)
 *	@param[in] n_w_param je prvni parametr zpravy
 *	@param[in] n_l_param je druhy parametr zpravy
 *	@return vraci 0, pokud zprava byla zpracovana aplikaci, pripadne vraci vysledek DefWindowProc()
 */
LRESULT CALLBACK WndProc(HWND h_wnd, UINT n_msg, WPARAM n_w_param, LPARAM n_l_param)
{
	HDC hDC;
	PAINTSTRUCT Ps;

	switch(n_msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		b_running = false;
		return 0;

	case WM_PAINT:
		hDC = BeginPaint(h_wnd, &Ps);
		TextOut(hDC, 50, 42, "Johnny Carson", 13);
		EndPaint(h_wnd, &Ps);
		break;

	case WM_SIZE:
		{
			RECT t_wnd_area;
			GetClientRect(h_wnd, &t_wnd_area);
			int n_ww = t_wnd_area.right - t_wnd_area.left;
			int n_wh = t_wnd_area.bottom - t_wnd_area.top;
			onResize(0, 0, n_ww, n_wh);
		}
		return 0;

	case WM_LBUTTONDOWN:
		{
		POINTS pts = MAKEPOINTS(n_l_param);
		std::cout << "x:" << pts.x << " | y:" << pts.y << std::endl;//0,0				800,0
																	//0,600				800,600
		pointsQueue.push_back(pts);
		break;
		}

	case WM_KEYUP:

		//smery pohybu
		if(n_w_param == VK_RIGHT)
			key.right = false;
		
		if(n_w_param == VK_LEFT)
			key.left = false;

		if(n_w_param == VK_UP)
			key.up = false;
		
		if(n_w_param == VK_DOWN)
			key.down = false;

		if(n_w_param == VK_SPACE)
			key.space = false;

		if(n_w_param == VK_NUMPAD0)
			key.reset = false;

		break;


	case WM_KEYDOWN:
		if(n_w_param == VK_ESCAPE)
			SendMessage(h_wnd, WM_CLOSE, 0, 0);

		//smery pohybu
		if(n_w_param == VK_RIGHT)
			key.right = true;
		
		if(n_w_param == VK_LEFT)
			key.left = true;
		
		if(n_w_param == VK_UP)
			key.up = true;

		if(n_w_param == VK_DOWN)
			key.down = true;

		if(n_w_param == VK_SPACE)
			key.space = true;

		if(n_w_param == VK_NUMPAD0)
			key.reset = true;

		break;

		return 0;
	}

	return DefWindowProc(h_wnd, n_msg, n_w_param, n_l_param) ;
}

/**
 *	@brief notifikace zmeny velikosti okna
 *	@param[in] n_x je x-souradnice leveho horniho rohu (vzdy 0)
 *	@param[in] n_y je y-souradnice leveho horniho rohu (vzdy 0)
 *	@param[in] n_new_width je nova sirka okna (klientska cast)
 *	@param[in] n_new_height je nova vyska okna (klientska cast)
 */
void onResize(int n_x, int n_y, int n_new_width, int n_new_height)
{
	n_width = n_new_width;
	n_height = n_new_height;
	glViewport(n_x, n_y, n_new_width, n_new_height);
}


void simulateSound(int textnum, int speed){

	for(int i = 0; i < speed; i++){	

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthmap);
		glActiveTexture(GL_TEXTURE0);

		velocityShader.bind(datawidth, dataheight);	drawPingPong(textnum);
		heightShader.bind(datawidth, dataheight);	drawPingPong(textnum);
	}
}

void drawPingPong(int textnum){

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_texture[textnum][1 - n_source_tex], 0); // kresleni do nulte urovne textury

	glViewport(0, 0, datawidth, dataheight); // viewport na velikost FBO
	glClear ( GL_COLOR_BUFFER_BIT);

	glBindTexture(GL_TEXTURE_2D, n_texture[textnum][n_source_tex]);
	DrawScene();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, n_texture[textnum][1 - n_source_tex]);
	glGenerateMipmap(GL_TEXTURE_2D);

	n_source_tex = 1 - n_source_tex; // ping-pong
}

bool reachedExit(Box heroBox){

	Box levelExit = levels.at(currentLevelIndex).exit;
	//pokud exit box lezi v hero box
	if(levelExit.vMin.x >= heroBox.vMin.x && levelExit.vMin.y >= heroBox.vMin.y && levelExit.vMax.x <= heroBox.vMax.x && levelExit.vMax.y <= heroBox.vMax.y)
		return true;
	else
		return false;
}

Level changeLevel(){
	currentLevelIndex++;
	Level currentLevel;

	if(currentLevelIndex+1 > levels.size()){
		std::cout << "dohrany vsechny urovne, vracim na zacatek" << std::endl;
		currentLevelIndex = 0;
	}
	
	currentLevel = levels.at(currentLevelIndex);
		
	//fyzika
	TBmp * bitmap;
	if(!(bitmap = CPngCodec::p_Load_PNG(currentLevel.mapFilepath.c_str()))){
		fprintf(stderr, "error: failed to load \'.png file\'\n");
		//return false;
	}
	boxDebug.generate(bitmap);
	depthmap = n_GLTextureFromBitmap(bitmap, GL_RGBA8);
	bitmap->Delete();

	//cisteni vln
	clearTexture(0);
	clearTexture(1);
	clearTexture(2);

	hero.position = currentLevel.heroStartPosition;

	return currentLevel;
	
}

void resetLevel(Hero * hero){

	hero->moving = false;
	hero->jumping = false;
	hero->velocity.x = 0.0f;
	hero->velocity.y = 0.0f;

	clearTexture(0);
	clearTexture(1);
	clearTexture(2);
}

/**
 *	@brief tato funkce se vola ve smycce pokazde kdyz nejsou zadne nove zpravy; lze vyuzit ke kresleni animace
 *	@param[in] driver je reference na OpenGL driver
 */
void OnIdle(CGL30Driver &driver)
{
	glClearColor(0, 0, 0, 0); //barva pozadi
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);//vypnuto protoze kreslime podle poradi (ne podle hloubek - vsechno na stejne Z)
	// vycistime framebuffer

	glBindTexture(GL_TEXTURE_2D, 0);//nechci omylem vykreslovat posledni bind. texturu
	
	simulateSound(0, 3);	//hrac - zdroj zvuku	
	simulateSound(1, 3);	//prostredi - zdroj zvuku
	simulateSound(2, 15);	//prostredi - vychod

	glViewport(0, 0, n_width, n_height); // viewport na velikost okna
	glClear ( GL_COLOR_BUFFER_BIT);

	Matrix4f t_mvp;
	{
		Matrix4f t_projection;
		CGLTransform::Perspective(t_projection, 90, float(n_width) / n_height, .01f, 1000);
		Matrix4f t_modelview;
		t_modelview.Identity();
		t_modelview.Translate(0, 0, -2.5f);
		t_mvp = t_projection * t_modelview;
	}

	glBindTexture(GL_TEXTURE_2D, n_texture[0][n_source_tex]);
	glm::vec3 color = glm::vec3(0.4, 1.0, 0.8);
	passShader.bind(t_mvp, color); // ted kreslite do okna, chcete videt vysledek
	DrawScene();

	glBindTexture(GL_TEXTURE_2D, n_texture[1][n_source_tex]);
	glm::vec3 color2 = glm::vec3(1.0, 0.2, 0.2);
	passShader.bind(t_mvp, color2);
	DrawScene();

	glBindTexture(GL_TEXTURE_2D, n_texture[2][n_source_tex]);
	glm::vec3 color3 = glm::vec3(0.4, 0.2, 1.0);
	passShader.bind(t_mvp, color3);
	DrawScene();
	
	//pridani kapek
	while(pointsQueue.size() > 0){
		POINTS p = pointsQueue.at(0);
		pointsQueue.pop_back();
		float x = (float)p.x/datawidth * 2.0f - 1.0f;
		float y = 1.0f - (float)p.y/dataheight * 2.0f;
		std::cout << "kapka na souradnicich - px: " << p.x << " |py: " << p.y << std::endl;
		//std::cout << "kapka na souradnicich - x: " << x << " |y: " << y << std::endl;
		drawDrop(false, x, y);
	}

	Level currentLevel = levels.at(currentLevelIndex);

	//vychod
	float flickering = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if(flickering > 0.98f)
		drawDrop2(false, currentLevel.exit.vMin.x+0.5f, currentLevel.exit.vMin.y+0.5f, 2);

	//zdroj zvuku	
	for(unsigned int i = 0; i < currentLevel.soundSourcePositions.size(); i++){
		flickering = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if(flickering > 0.99f)
			drawDrop2(false, currentLevel.soundSourcePositions.at(i).x, currentLevel.soundSourcePositions.at(i).y, 1);
	}

	//tukani hulkou
	if(key.space){
		if(hero.facingLeft)
			drawDrop2(false, hero.position.x, hero.position.y + hero.height, 0);
		else
			drawDrop2(false, hero.position.x + hero.width, hero.position.y + hero.height, 0);
	}

	//skok-vkladani kapek pro testovani
	if(key.up){
		drawDrop2(false, hero.position.x + hero.width/2.0f, hero.position.y + hero.height, 0);
	}

	//nastavit novy pohyb na zaklade stisknutych tlacitek
	boxDebug.calculateTimeBasedMovement(frame_time, key, &hero);
	hero.updateBox();

	//zjistit kolize pro novy pohyb
	//poupravit novy pohyb, aby nedoslo ke kolizi
	boxDebug.SeparatingAxisTheorem(&hero);

	//vykreslit postavu
	hero.draw();

	//vykreslit geometrii
	//boxDebug.draw();
	glm::vec3 heroBoxColor(1.0, 0.6, 0.4);
	//boxDebug.drawBox(hero.box, heroBoxColor);
	glm::vec3 levelExitColor(0.4, 0.6, 0.8);
	//boxDebug.drawBox(currentLevel.exit, levelExitColor);

	//vykreslit hmat
	boxDebug.drawCollisions();

	boxDebug.movementDirection.x = 0.0;
	boxDebug.movementDirection.y = 0.0;

	if(reachedExit(hero.box)){
		std::cout << "menim level" << std::endl;
		currentLevel = changeLevel();
	}

	if(key.reset){
		std::cout << " RESET " << std::endl;
		resetLevel(&hero);
		hero.position = currentLevel.heroStartPosition;
	}
	driver.Blit(); // preklopi buffery, zobrazi co jsme nakreslili
}
