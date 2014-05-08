#include "Texture.h"

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

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	glTexImage2D(GL_TEXTURE_2D, 0, n_internal_format, p_bitmap->n_width, p_bitmap->n_height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, p_bitmap->p_buffer);
	// specifikujeme data textury

	glGenerateMipmap(GL_TEXTURE_2D);
	// nechame OpenGL spocitat jeji mipmapy

	return n_tex;
}

/**
 *	@brief vraci texturu nactenou ze souboru ve formatu png
 *	@param[in] imagepath jmeno souboru
 */
GLuint LoadTextureFromPNGFile(const char * imagepath){

	TBmp * bitmap;
	if(!(bitmap = CPngCodec::p_Load_PNG(imagepath))){
		fprintf(stderr, "error: failed to load \'.png file\'\n");
		system("PAUSE");
		return false;
	}

	GLuint texture = n_GLTextureFromBitmap(bitmap, GL_RGBA8);
	bitmap->Delete();

	return texture;	
}

/**
 *	@brief vraci texturu nactenou ze souboru ve formatu tga
 *	@param[in] imagepath jmeno souboru
 */
GLuint LoadTextureFromTGAFile(const char * imagepath){
	TBmp * bitmap;
	if(!(bitmap = CTgaCodec::p_Load_TGA(imagepath))) {
		fprintf(stderr, "error: failed to load \'.tga file\'\n");
		return false;
	}

	GLuint texture = n_GLTextureFromBitmap(bitmap, GL_RGBA8);
	bitmap->Delete();

	return texture;
}

/**
 *	@brief testovaci funkce
 */
GLuint n_GLTextureRFloatFromData()
{
	GLuint n_tex_surface;
	GLfloat *checkImage;

	//alokace pameti
	try {
		checkImage = new GLfloat [datawidth * dataheight];
	} catch (std::bad_alloc e){
		std::cerr << "Alokace pameti selhala" << std::endl;
		return -1;
	}

	//pocatecni data
	float r = 1.0f;
	for (int i = 0; i < dataheight * datawidth; i++) {
			//r = r + 0.0001f;
			checkImage[i] = (GLfloat) r;
	}

	int x = datawidth/2;
	int y = dataheight/2;
	checkImage[x + datawidth * y] = 0.0f;

	for(int x = 0; x < datawidth; x++){
		for(int y = 0; y < dataheight; y++){
			if(x > datawidth/3)
				checkImage[x + datawidth * y] = 0.5f;
			if(y == dataheight/5)
				checkImage[x + datawidth * y] = 0.0f;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, datawidth, dataheight, 0, GL_RED, GL_FLOAT, checkImage);

	//uvolneni pameti
	delete[] checkImage;

	return n_tex_surface;
}

/**
 *	@brief testovaci funkce
 */
GLuint n_GLTextureRFloatDepthTest()
{
	GLuint n_tex_surface;
	GLfloat *checkImage;
	int wid=21;
	int hei=16;

	//alokace pameti
	try {
		checkImage = new GLfloat [21 * 16];
	} catch (std::bad_alloc e){
		std::cerr << "Alokace pameti selhala" << std::endl;
		return -1;
	}

	//pocatecni data
	float r = 7.0;
	for (int y = 0; y < hei; y++) {
		for (int x = 0; x < wid; x++) {		
			checkImage[x + wid * y] = (GLfloat) r;		
			if(x >= 5 && x < 8 && y >= 8 && y <11)
				checkImage[x + wid * y] = -5.0;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, wid, hei, 0, GL_RED, GL_FLOAT, checkImage);

	//uvolneni pameti
	delete[] checkImage;

	return n_tex_surface;
}

/**
 *	@brief vytvori texturu s float cervenym kanalem
 */
GLuint n_GLTextureRFloat()
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

/**
 *	@brief vytvori texturu s float cervenym a zelenym kanalem
 */
GLuint n_GLTextureRGFloat()
{
	GLuint n_tex_surface;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, datawidth, dataheight, 0, GL_RG, GL_FLOAT, NULL);//null

	return n_tex_surface;
}

/**
 *	@brief vytvori texturu s float cervenym,zelenym a modrym kanalem
 */
GLuint n_GLTextureRGBFloat()
{
	GLuint n_tex_surface;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, datawidth, dataheight, 0, GL_RGB, GL_FLOAT, NULL);//null

	return n_tex_surface;
}

/**
 *	@brief testovaci funkce
 */
GLuint n_GLTextureRGBFloatFromData()
{
	GLuint n_tex_surface;
	GLfloat *checkImage;

	//alokace pameti
	try {
		checkImage = new GLfloat [datawidth * dataheight * 3];
	} catch (std::bad_alloc e){
		std::cerr << "Alokace pameti selhala" << std::endl;
		return -1;
	}

	//pocatecni data
	float r = 0.0;
	srand (static_cast <unsigned> (time(0)));
	for (int y = 0; y < dataheight; y++) {
		for (int x = 0; x < datawidth; x++) {
			checkImage[(x + datawidth * y) * 3] = (GLfloat) r;//red
			checkImage[(x + datawidth * y) * 3 + 1] = 0.0;//(GLfloat) static_cast <float> (rand()) / static_cast <float> (RAND_MAX);; //green
			checkImage[(x + datawidth * y) * 3 + 2] = 0.0;//(GLfloat) static_cast <float> (rand()) / static_cast <float> (RAND_MAX);; //blue
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, datawidth, dataheight, 0, GL_RGB, GL_FLOAT, /*NULL*/checkImage);

	//uvolneni pameti
	delete[] checkImage;

	return n_tex_surface;
}