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

#include "OpenGL30Drv.h"
#include "Transform.h"
#include "Tga.h"
#include "Shader.h"
#include "Texture.h"
#include "Common.h"

static const char *p_s_window_name = "OpenGL 3 window";
static const char *p_s_class_name = "my_wndclass";
// jmeno okna

static int n_width = 800;
static int n_height = 600;
// velikost okna

static bool b_running = true;
// flag pro vyskoceni ze smycky zprav

void OnIdle(CGL30Driver &driver);
void onResize(int n_x, int n_y, int n_new_width, int n_new_height);
LRESULT CALLBACK WndProc(HWND h_wnd, UINT n_msg, WPARAM n_w_param, LPARAM n_l_param);
// prototypy funkci

GLuint fbo;
int n_source_tex;
static const GLenum p_draw_buffers[] = {GL_COLOR_ATTACHMENT0};
static GLuint n_vertex_buffer_object, n_index_buffer_object, n_vertex_array_object;

//textury
static GLuint n_texture[2];//r-height,g-y velocity,b-x velocity
static GLuint depthmap;

//shadery
ShaderBlur blur;
ShaderPass pass;
ShaderVelocity velocity;
ShaderHeight height;

/**
 *	@brief vraci "pointer" do VBO
 *	@param[in] off je offset v bytech, na ktery ma vraceny pointer ukazovat
 */
#define p_OffsetInVBO(off) ((void*)(off))

/**
 *	@brief vytvori vsechny OpenGL objekty, potrebne pro kresleni
 *	@return vraci true pri uspechu, false pri neuspechu
 */
bool InitGLObjects()
{

	const float p_box_vertices[] = {
	0, 0, -1, -1, 1, /**/1, 0, 1, -1, 1,/**/ 1, 1, 1, 1, 1,/**/ 0, 1, -1, 1, 1};
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
	blur.compile();
	pass.compile();
	velocity.compile();
	height.compile();

	/*
	TBmp *p_box;
	if(!(p_box = CTgaCodec::p_Load_TGA("box.tga"))) {
		fprintf(stderr, "error: failed to load \'box.tga\'\n");
		return false;
	}
	// nahraje obrazky
	*/

	//textury
	n_texture[1] = n_GLTextureRGBFloat();
	n_texture[0] = n_GLTextureRGBFloatFromData();
	depthmap = n_GLTextureRFloatDepthTest();

	//FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glDrawBuffers(1, p_draw_buffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	//init dat v texture
	/*
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_texture[0], 0);
	glViewport(0, 0, datawidth, dataheight);
	glClearColor(0.8, 0.8, 0.4, 0.0);
	glClear ( GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, n_texture[0]);
	*/

	n_source_tex = 0;
	
	//cisteni textury depthmap - meni barvu pozadi WTF?
	/*
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthmap, 0);
	glViewport(0, 0, datawidth, dataheight);
	glClearColor(1.0, 0, 0, 0); // jen na prvni hodnote zalezi - meni rgb barvu pozadi / opengl je stavovy
	glClear ( GL_COLOR_BUFFER_BIT);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	*/

	//cisteni textury depthmap - jiny zpusob
	/*
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depthmap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	GLuint clearColor[4] = {1, 0, 0, 0};
	glClearBufferuiv(GL_COLOR, 0, clearColor);*/
	

	

	//p_box->Delete();
	// uvolni pamet
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

	//glDeleteShader(n_vertex_shader);//TODO
	//glDeleteShader(n_fragment_shader);
	//glDeleteProgram(n_program_object);
	// smaze shadery

	glDeleteTextures(1, n_texture);
	// smaze textury
}

/**
 *	@brief vykresli geometrii sceny (bez specifikace cehokoliv jineho nez geometrie)
 */
void DrawScene()
{
	glBindVertexArray(n_vertex_array_object);
	// rekne OpenGL odkud se berou data pro kresleni (z nasich VBO)

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, p_OffsetInVBO(0));
	// nakresli objekt pomoci jedineho prikazu

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

	if(!InitGLObjects()) {
		fprintf(stderr, "error: failed to initialize OpenGL objects\n"); // neco se nepovedlo (chyba pri kompilaci shaderu / chyba pri nacitani obrazku textury)
		return -1;
	}
	// vyrobime objekty OpenGL, nutne ke kresleni

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
	}
	// smycka zprav, s funkci OnIdle()

	CleanupGLObjects();
	// uvolnime OpenGL objekty

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
	switch(n_msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		b_running = false;
		return 0;

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
		{/*
		//std::cout << "click mouse" << std::endl;
		// Capture mouse input. 
		RECT t_wnd_area;
		GetClientRect(h_wnd, &t_wnd_area);
		int n_ww = t_wnd_area.right - t_wnd_area.left;
		int n_wh = t_wnd_area.bottom - t_wnd_area.top;
		std::cout << "ww:" << n_ww << " | wh:" << n_wh << std::endl;//800 600
		*/
		POINTS pts = MAKEPOINTS(n_l_param);
		std::cout << "x:" << pts.x << " | y:" << pts.y << std::endl;//0,0				800,0
																	//0,600				800,600
		break;
		}

	case WM_KEYDOWN:
		if(n_w_param == VK_ESCAPE)
			SendMessage(h_wnd, WM_CLOSE, 0, 0);
		if(n_w_param == VK_SPACE)
			std::cout << "space" << std::endl;
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


void drawPingPong(){

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_texture[1 - n_source_tex], 0); // kresleni do nulte urovne textury

	glViewport(0, 0, datawidth, dataheight); // viewport na velikost FBO
	glClear ( GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, n_texture[n_source_tex]);

	DrawScene();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, n_texture[1 - n_source_tex]);

	glGenerateMipmap(GL_TEXTURE_2D);

	n_source_tex = 1 - n_source_tex; // ping-pong
}

/**
 *	@brief tato funkce se vola ve smycce pokazde kdyz nejsou zadne nove zpravy; lze vyuzit ke kresleni animace
 *	@param[in] driver je reference na OpenGL driver
 */
void OnIdle(CGL30Driver &driver)
{
	glClearColor(0, 0, 0, 0); //barva pozadi
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// vycistime framebuffer

	glBindTexture(GL_TEXTURE_2D, 0);//nechci omylem vykreslovat posledni bind. texturu

	float f_time = clock() / 1000.0f; // cas pro animaci

	//volani shaderu
	//blur.bind(datawidth, dataheight);	drawPingPong();

	
	/*
	if(clickCount > 0){
		std::cout << "klik x: y: " << std::endl;
	}*/

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthmap);
	glActiveTexture(GL_TEXTURE0);

	velocity.bind(datawidth, dataheight);	drawPingPong();
	height.bind(datawidth, dataheight);	drawPingPong();

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

	pass.bind(t_mvp); // ted kreslite do okna, chcete videt vysledek
	DrawScene();

	driver.Blit(); // preklopi buffery, zobrazi co jsme nakreslili

	//system("PAUSE");
}
