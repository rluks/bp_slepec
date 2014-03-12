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

GLuint n_GLTextureRFloatDepthTest()
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

	/*
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
	}*/

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
	/*
	float r = 1.0f;
	for (int i = 0; i < dataheight * datawidth * 3; i++) {
		checkImage[i] = (GLfloat) r;
	}*/

	//rgbrgbrgbrgb...
	float r = 1.0f;
	int cnt = 0;
	for (int i = 0; i < dataheight * datawidth * 3; i++) {
		checkImage[i] = (GLfloat) r;

		if(r == 1.0f){
			r = 0.5f;
			if(cnt % 100 == 0)
				checkImage[i] = 0.2f;;
		}
		else if(r == 0.5f)
			r = 0.0f;
		else
			r = 1.0f;

		cnt++;
	}

	/*
	//rgbrgbrgbrgb...
	float r = 1.0f;
	for (int i = 0; i < dataheight * datawidth * 3; i++) {
		checkImage[i] = (GLfloat) r;

		if(r == 1.0f)
			r = 0.5f;
		else if(r == 0.5f)
			r = 0.0f;
		else
			r = 1.0f;
	}*/

	/* vlajka >:->
	for (int i = 0; i < dataheight * datawidth; i++)
		checkImage[i] = 1.0f;

	for (int i = dataheight * datawidth; i < dataheight * datawidth * 2; i++)
		checkImage[i] = 0.5f;

	for (int i = dataheight * datawidth * 2; i < dataheight * datawidth * 3; i++)
		checkImage[i] = 0.1f;
	*/

	int x = datawidth/2;
	int y = dataheight/2;
	checkImage[x + datawidth * y] = 5.0f;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, datawidth, dataheight, 0, GL_RGB, GL_FLOAT, checkImage);

	//uvolneni pameti
	delete[] checkImage;

	return n_tex_surface;
}