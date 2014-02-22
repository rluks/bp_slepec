#include <GL/glew.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "OpenGL30Drv.h"
#include "Transform.h"
#include "Tga.h"


#include <istream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include <cstdlib>
#include <ctime>

#define datawidth 100
#define dataheight 100

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
static GLuint n_box_texture[2];

static GLuint n_surface_texture;//textura hladiny

static GLuint n_vertex_buffer_object, n_index_buffer_object, n_vertex_array_object;
static GLuint n_vertex_shader, n_fragment_shader, n_program_object, n_mvp_matrix_uniform,
	n_box_sampler_uniform, n_time_uniform, n_surface_sampler_uniform;
// OpenGL objekty

static GLuint n_vertex_shader_pass, n_fragment_shader_pass;
static GLuint n_program_object_pass;
static GLuint 
	n_mvp_matrix_uniform_pass, 
	n_box_sampler_uniform_pass, 
	n_time_uniform_pass;

/**
 *	@brief zkontroluje zda byl shader spravne zkompilovan, tiskne chyby do stderr
 *	@param[in] n_shader_object je id shaderu
 *	@param[in] p_s_shader_name je jmeno shaderu (jen pro uzivatele - aby vedel o ktery shader se jedna pokud se vytiskne nejaka chyba)
 *	@return vraci true pro uspesne zkompilovane shadery, jinak false
 */
bool CheckShader(GLuint n_shader_object, const char *p_s_shader_name)
{
	bool b_compiled;
	{
		int n_tmp;
		glGetShaderiv(n_shader_object, GL_COMPILE_STATUS, &n_tmp);
		b_compiled = n_tmp == GL_TRUE;
	}
	int n_log_length;
	glGetShaderiv(n_shader_object, GL_INFO_LOG_LENGTH, &n_log_length);
	// query status ...

	if(n_log_length > 1) {
		char *p_s_info_log;
		if(!(p_s_info_log = new(std::nothrow) char[n_log_length + 1]))
			return false;
		// alloc temp storage for log string

		glGetShaderInfoLog(n_shader_object, n_log_length, &n_log_length, p_s_info_log);
		// get log string

		printf("GLSL compiler (%s): %s\n", p_s_shader_name, p_s_info_log);
		// print info log

		delete[] p_s_info_log;
		// delete temp storage
	}
	// get info-log

	return b_compiled;
}

/**
 *	@brief zkontroluje zda byl program spravne slinkovan, tiskne chyby do stderr
 *	@param[in] n_program_object je id programu
 *	@param[in] p_s_program_name je jmeno programu (jen pro uzivatele - aby vedel o ktery program se jedna pokud se vytiskne nejaka chyba)
 *	@return vraci true pro uspesne slinkovne programy, jinak false
 */
bool CheckProgram(GLuint n_program_object, const char *p_s_program_name)
{
	bool b_linked;
	{
		int n_tmp;
		glGetProgramiv(n_program_object, GL_LINK_STATUS, &n_tmp);
		b_linked = n_tmp == GL_TRUE;
	}
	int n_length;
	glGetProgramiv(n_program_object, GL_INFO_LOG_LENGTH, &n_length);
	// query status ...

	if(n_length > 1) {
		char *p_s_info_log;
		if(!(p_s_info_log = new(std::nothrow) char[n_length + 1]))
			return false;
		// alloc temp log

		glGetProgramInfoLog(n_program_object, n_length, &n_length, p_s_info_log);
		// get info log

		printf("GLSL linker (%s): %s\n", p_s_program_name, p_s_info_log);
		// print info log

		delete[] p_s_info_log;
		// release temp log
	}
	// get info-log

	return b_linked;
}

/**
 *	@brief vyrobi OpenGL texturu z bitmapy
 *	@param[in] p_bitmap je obrazek textury
 *	@param[in] n_internal_format je pozadovany format ulozeni textury na graficke karte (format obrazku je v typu TBmp vzdy GL_RGBA)
 *	@return vraci id textury (neuvazuje neuspech)
 */
GLuint n_GLTextureFromBitmap(const TBmp *p_bitmap, GLenum n_internal_format = GL_RGBA8)
{
	GLuint n_tex;
	glGenTextures(1, &n_tex);
	glBindTexture(GL_TEXTURE_2D, n_tex);
	//glEnable(GL_TEXTURE_2D); !! OpenGL 3 uz nema enable / disable textur, shader si sam cte textury ktere potrebuje !!
	// vyrobi a nastavi novou 2D texturu, zatim nema zadne data

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// nastavime filtrovani

	glTexImage2D(GL_TEXTURE_2D, 0, n_internal_format, p_bitmap->n_width, p_bitmap->n_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, p_bitmap->p_buffer);
	// specifikujeme data textury

	glGenerateMipmap(GL_TEXTURE_2D);
	// nechame OpenGL spocitat jeji mipmapy

	return n_tex;
}

GLuint n_GLTextureFromData()
{

	GLuint n_tex_surface;
	GLfloat checkImage[dataheight][datawidth];

	float r = 1.0f;
	for (int i = 0; i < dataheight; i++) {
		for (int j = 0; j < datawidth; j++) {
			//r = r + 0.0001f;
			checkImage[i][j] = (GLfloat) r;
		}
	}

	
	for (int i = 0; i < dataheight; i++)
		for (int j = 0; j < datawidth; j++)
			checkImage[j][j] = 0.0f;


	for (int i = 0; i < dataheight; i++)
		checkImage[i][20] = 0.0f;

	for (int j = 0; j < datawidth; j++)
		checkImage[5][j] = 0.0f;


	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, datawidth, dataheight, 0, GL_RED, GL_FLOAT, checkImage);

	return n_tex_surface;
}

GLuint n_GLTextureEmpty()
{
	GLuint n_tex_surface;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, datawidth, dataheight, 0, GL_RED, GL_FLOAT, NULL);//null

	return n_tex_surface;
}

std::string getTextFileString(std::string name){

	FILE* fp = fopen(name.c_str(), "rt");
	if(!fp){
		std::cerr << "Cannot load file" << std::endl;
		system("PAUSE");
		return false;
	}

	//std::vector<std::string> sLines;
	std::string text;
	char sLine[255];
	while(fgets(sLine, 255, fp))text.append(sLine);
	fclose(fp);

	return text;
}

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
	0, 1, -1, -1, 1, /**/1, 1, 1, -1, 1,/**/ 1, 0, 1, 1, 1,/**/ 0, 0, -1, 1, 1};
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

	//shader na processing
	std::string vertexString = getTextFileString("vertex.shader");
	const char *p_s_vertex_shader = vertexString.c_str();

	std::string fragmentString = getTextFileString("fragment.shader");
	const char *p_s_fragment_shader = fragmentString.c_str();
	// zdrojove kody pro vertex / fragment shader (OpenGL 3 uz nepouziva specifikator 'verying', jinak je kod stejny)

	n_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(n_vertex_shader, 1, &p_s_vertex_shader, NULL);
	glCompileShader(n_vertex_shader);
	if(!CheckShader(n_vertex_shader, "vertex shader"))
		return false;
	n_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(n_fragment_shader, 1, &p_s_fragment_shader, NULL);
	glCompileShader(n_fragment_shader);
	if(!CheckShader(n_fragment_shader, "fragment shader"))
		return false;
	n_program_object = glCreateProgram();
	glAttachShader(n_program_object, n_vertex_shader);
	glAttachShader(n_program_object, n_fragment_shader);
	// zkompiluje vertex / fragment shader, pripoji je k programu

	glBindAttribLocation(n_program_object, 0, "v_tex");//2hodnoty vec2
	glBindAttribLocation(n_program_object, 1, "v_pos");//3hodnoty vec3
	// nabinduje atributy (propojeni mezi obsahem VBO a vstupem do vertex shaderu)

	glBindFragDataLocation(n_program_object, 0, "frag_color");
	// nabinduje vystupni promenou (propojeni mezi framebufferem a vystupem fragment shaderu)

	glLinkProgram(n_program_object);
	if(!CheckProgram(n_program_object, "program"))
		return false;
	// slinkuje program

	n_mvp_matrix_uniform = glGetUniformLocation(n_program_object, "t_modelview_projection_matrix");
	n_box_sampler_uniform = glGetUniformLocation(n_program_object, "n_box_tex");
	n_time_uniform = glGetUniformLocation(n_program_object, "f_time");
	// najde cislo parametru shaderu podle jeho jmena

	//shadery na ping pong
	std::string vertexStringPass = getTextFileString("vertex_pass.shader");
	const char *p_s_vertex_shader_pass = vertexStringPass.c_str();
	std::string fragmentStringPass = getTextFileString("fragment_pass.shader");
	const char *p_s_fragment_shader_pass = fragmentStringPass.c_str();

	n_vertex_shader_pass = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(n_vertex_shader_pass, 1, &p_s_vertex_shader_pass, NULL);
	glCompileShader(n_vertex_shader_pass);
	if(!CheckShader(n_vertex_shader_pass, "vertex shader pass"))
		return false;
	n_fragment_shader_pass = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(n_fragment_shader_pass, 1, &p_s_fragment_shader_pass, NULL);
	glCompileShader(n_fragment_shader_pass);
	if(!CheckShader(n_fragment_shader_pass, "fragment shader pass"))
		return false;
	n_program_object_pass = glCreateProgram();
	glAttachShader(n_program_object_pass, n_vertex_shader_pass);
	glAttachShader(n_program_object_pass, n_fragment_shader_pass);
	// zkompiluje vertex / fragment shader, pripoji je k programu

	glBindAttribLocation(n_program_object_pass, 0, "v_tex");//2hodnoty vec2
	glBindAttribLocation(n_program_object_pass, 1, "v_pos");//3hodnoty vec3
	// nabinduje atributy (propojeni mezi obsahem VBO a vstupem do vertex shaderu)

	glBindFragDataLocation(n_program_object_pass, 0, "frag_color");
	// nabinduje vystupni promenou (propojeni mezi framebufferem a vystupem fragment shaderu)

	glLinkProgram(n_program_object_pass);
	if(!CheckProgram(n_program_object_pass, "program pass"))
		return false;
	// slinkuje program

	n_mvp_matrix_uniform_pass = glGetUniformLocation(n_program_object_pass, "t_modelview_projection_matrix");
	n_box_sampler_uniform_pass = glGetUniformLocation(n_program_object_pass, "n_box_tex");
	n_time_uniform_pass = glGetUniformLocation(n_program_object_pass, "f_time");

	TBmp *p_box;
	if(!(p_box = CTgaCodec::p_Load_TGA("box.tga"))) {
		fprintf(stderr, "error: failed to load \'box.tga\'\n");
		return false;
	}
	// nahraje obrazky

	n_box_texture[1] = n_GLTextureEmpty();
	n_box_texture[0] = n_GLTextureFromData();
	// vyrobi textury

	//FBO
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glDrawBuffers(1, p_draw_buffers);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	n_source_tex = 0;

	p_box->Delete();
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

	glDeleteShader(n_vertex_shader);
	glDeleteShader(n_fragment_shader);
	glDeleteProgram(n_program_object);
	// smaze shadery

	glDeleteTextures(1, n_box_texture);
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

	case WM_KEYDOWN:
		if(n_w_param == VK_ESCAPE)
			SendMessage(h_wnd, WM_CLOSE, 0, 0);
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

/**
 *	@brief tato funkce se vola ve smycce pokazde kdyz nejsou zadne nove zpravy; lze vyuzit ke kresleni animace
 *	@param[in] driver je reference na OpenGL driver
 */
void OnIdle(CGL30Driver &driver)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// vycistime framebuffer

	float f_time = clock() / 1000.0f; // cas pro animaci
	
	system("PAUSE");

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);
	glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, n_box_texture[1 - n_source_tex], 0); // kresleni do nulte urovne textury

	//kresleni do fbo
	glViewport(0, 0, datawidth, dataheight); // viewport na velikost FBO
	glClear ( GL_COLOR_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, n_box_texture[n_source_tex]); // ta druha textura

	glUseProgram(n_program_object);
	{
		glUniform1i(n_box_sampler_uniform, 0);
	}

	DrawScene();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // chcete kreslit do okna
	glViewport(0, 0, n_width, n_height); // viewport na velikost okna

	glBindTexture(GL_TEXTURE_2D, n_box_texture[1 - n_source_tex]); // textura do ktere se kreslilo
	glGenerateMipmap(GL_TEXTURE_2D);
	// musi se pregenerovat mipmapy

	n_source_tex = 1 - n_source_tex; // ping-pong
	// nova data jsou ted v n_source_tex

	glClear(GL_COLOR_BUFFER_BIT);
	//glBindTexture(GL_TEXTURE_2D, n_box_texture[n_source_tex]); // ta druha textura

	Matrix4f t_mvp;
	{
		Matrix4f t_projection;
		CGLTransform::Perspective(t_projection, 90, float(n_width) / n_height, .01f, 1000);
		// spocitame matici perspektivni projekce

		Matrix4f t_modelview;
		t_modelview.Identity();
		t_modelview.Translate(0, 0, -2.5f);
		// spocitame matici modelview

		t_mvp = t_projection * t_modelview;
	}
	// spocitame modelview - projection matici, kterou potrebujeme k transformaci vrcholu

	glUseProgram(n_program_object_pass); // bind shader (jen zobrazuje co je v texture)
// ted kreslite do okna, chcete videt vysledek
	{
		glUniformMatrix4fv(n_mvp_matrix_uniform_pass, 1, GL_FALSE, &t_mvp[0][0]);
		glUniform1i(n_box_sampler_uniform_pass, 0);
	}

	DrawScene(); // vykreslime objekt pomoci VBO
	
	driver.Blit(); // preklopi buffery, zobrazi co jsme nakreslili
}
