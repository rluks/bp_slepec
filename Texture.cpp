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

GLuint LoadTexture(const char* filename, const unsigned int texID, GLenum image_format, GLint internal_format, GLint level, GLint border)
{
	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to
	GLuint gl_texID;
	
	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if(fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if(!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
		return false;
	
	//if this texture ID is in use, unload the current texture
	/*
	if(m_texID.find(texID) != m_texID.end())
		glDeleteTextures(1, &(m_texID[texID]));*/

	//generate an OpenGL texture ID for this texture
	glGenTextures(1, &gl_texID);
	//store the texture ID mapping
	//m_texID[texID] = gl_texID;
	//bind to the new texture ID
	glBindTexture(GL_TEXTURE_2D, gl_texID);
	//store the texture data for OpenGL use
	glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height,
		border, image_format, GL_UNSIGNED_BYTE, bits);

	//Free FreeImage's copy of the data
	FreeImage_Unload(dib);

	//return success
	return gl_texID;
}

GLuint loadBMP_custom(const char * imagepath){

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data
	unsigned char * data;

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file - 54 bytes\n");
		return 0;
	}
	// A BMP files always begins with "BM"
	if ( header[0]!='B' || header[1]!='M' ){
		printf("Not a correct BMP file - begins with BM\n");
		return 0;
	}
	// Make sure this is a 24bpp file
	if ( *(int*)&(header[0x1E])!=0  )         {printf("Not a correct BMP file 24bpp 0\n");    return 0;}
	if ( *(int*)&(header[0x1C])!=24 )         {printf("Not a correct BMP file 24bpp 24\n");    return 0;}

	// Read the information about the image
	dataPos    = *(int*)&(header[0x0A]);
	imageSize  = *(int*)&(header[0x22]);
	width      = *(int*)&(header[0x12]);
	height     = *(int*)&(header[0x16]);

	// Some BMP files are misformatted, guess missing information
	if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
	if (dataPos==0)      dataPos=54; // The BMP header is done that way

	// Create a buffer
	data = new unsigned char [imageSize];

	// Read the actual data from the file into the buffer
	fread(data,1,imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;

	// Poor filtering, or ...
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
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
	/*
	float r = 1.0f;
	for (int i = 0; i < dataheight * datawidth; i++) {
			//r = r + 0.0001f;
			checkImage[i] = (GLfloat) r;
	}*/

	
	float r = 7.0;
	for (int y = 0; y < hei; y++) {
		for (int x = 0; x < wid; x++) {		
			checkImage[x + wid * y] = (GLfloat) r;		
			if(x >= 5 && x < 8 && y >= 8 && y <11)
				checkImage[x + wid * y] = -5.0;
		}
	}

	/*
	for (int y = 0; y < dataheight; y++)
		for (int x = 0; x < datawidth; x++)	
			checkImage[x + datawidth * y] = 7.0;
	*/

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &n_tex_surface);

	glBindTexture(GL_TEXTURE_2D, n_tex_surface);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, wid, hei, 0, GL_RED, GL_FLOAT, checkImage);

	//uvolneni pameti
	delete[] checkImage;

	//glBindTexture(GL_TEXTURE_2D,0);

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

	/*
	srand (static_cast <unsigned> (time(0)));
	for (int i = 0; i < dataheight * datawidth * 3; i++) {
		checkImage[i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}*/

	/*
	for (int i = 400; i < dataheight * datawidth; i++) {
		checkImage[i] = 0.7;
	}*/

	/*
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
	}*/

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

	/*
	// vlajka >:->
	for (int i = 0; i < dataheight * datawidth; i++)
		checkImage[i] = 1.0f;

	for (int i = dataheight * datawidth; i < dataheight * datawidth * 2; i++)
		checkImage[i] = 0.5f;

	for (int i = dataheight * datawidth * 2; i < dataheight * datawidth * 3; i++)
		checkImage[i] = 0.1f;
		*/
	

	float r = 0.0;
	srand (static_cast <unsigned> (time(0)));
	for (int y = 0; y < dataheight; y++) {
		for (int x = 0; x < datawidth; x++) {
			checkImage[(x + datawidth * y) * 3] = (GLfloat) r;//red
			checkImage[(x + datawidth * y) * 3 + 1] = 0.0;//(GLfloat) static_cast <float> (rand()) / static_cast <float> (RAND_MAX);; //green
			checkImage[(x + datawidth * y) * 3 + 2] = 0.0;//(GLfloat) static_cast <float> (rand()) / static_cast <float> (RAND_MAX);; //blue
		}
	}
	
	/*
	int dropletRadius = 40;
	float da = (float)0.7;
	const int dim = 161;//4*dropletRadius+1;
    float xd[dim][dim];
    float yd[dim][dim];
    for(int i=0; i<dim; i++) {
		for(int j=-2*dropletRadius; j<=2*dropletRadius; j++){
			xd[i][j+2*dropletRadius] = (float)j;
			yd[j+2*dropletRadius][i] = (float)j;
        }
    }
    float m_Zd[dim][dim];
    for(int i=0; i<dim; i++) {
		for(int j=0; j<dim; j++) {
			m_Zd[i][j] = -da*exp(-pow(xd[i][j]/dropletRadius,2)-pow(yd[i][j]/dropletRadius,2));
			}
    }
	//m_Zd[0][0] = -1;
	
	//pridani kapky
	for(int i=0; i<dim; i++) {
        for(int j=0; j<dim; j++) {
			int x = datawidth/2 - dim/2 + i;
			int y = dataheight/2 - dim/2 + j;
			checkImage[(x + datawidth * y) * 3] +=  m_Zd[i][j];
		}
	}*/

	/* do jedne datawidth pruh dole
	for (int i = 0; i < datawidth * 3; i+=3) {
		float r = 0.2;
		checkImage[i] = (GLfloat) r;	
	}*/

	/* funguje
	for (int y = 0; y < dataheight; y++) {
		for (int x = 0; x < datawidth; x++) {
			float r = 0.2;
			checkImage[(x + datawidth * y)*3] = (GLfloat) r;
		}
	}*/

	/* taky funguje
	for (int y = 0; y < dataheight; y++) {
		for (int x = 0; x < datawidth * 3; x+=3) {
			float r = 0.2;
			checkImage[x + datawidth * 3 * y] = (GLfloat) r;
		}
	}*/
	
	/*
	srand (static_cast <unsigned> (time(0)));
	for (int i = 0; i < dataheight * datawidth * 3; i+=3) {
		float r;
		//red
		r= static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		checkImage[i] = (GLfloat) r;	
		//green
		r=0.0;
		checkImage[i+1] = (GLfloat) r;
		//blue
		r=0.0;
		checkImage[i+2] = (GLfloat) r;
	}*/

	/*
	for (int i = 0; i < dataheight * datawidth * 3; i+=3) {
		float r=0.8;;
		//red
		checkImage[i] = (GLfloat) r;	
		//green
		checkImage[i+1] = (GLfloat) r;
		//blue
		checkImage[i+2] = (GLfloat) r;
	}*/

	/*
	int x = datawidth/2;
	int y = dataheight/2;
	for(int i = 0; i < 50; i++){
		checkImage[x + datawidth * y] = 5.0f;
		checkImage[x+1 + datawidth * y] = 5.0f;
		checkImage[x+1 + datawidth * y+1] = 5.0f;
		checkImage[x+1 + datawidth * y-1] = 5.0f;
		checkImage[x-1 + datawidth * y] = 5.0f;
		checkImage[x-1 + datawidth * y+1] = 5.0f;
		checkImage[x-1 + datawidth * y-1] = 5.0f;
		checkImage[x + datawidth * y+1] = 5.0f;
		checkImage[x + datawidth * y-1] = 5.0f;
	}*/

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