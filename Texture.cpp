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

	////
	/*
	for(int i = 0; i < width*height; i++){
		unsigned char pixel = data[i];
		char out = (pixel == 0x00) ? ' ' : '*'; 
		printf("|%d|", pixel);
	}*/
	/*
	for(int i = 0; i < width; i++){
		std::string line;
		for(int j = 0; j < height; j++){
			unsigned char pixel = data[i + width * j];
			char out = (pixel == 0xFF) ? ' ' : '*'; 
			//std::cout << std::hex << pixel << std::endl;
			//printf("|%d|", pixel);
			line+=out;
		}
		std::cout << "|" << line << "|" << std::endl;
		line.clear();
	}
	std::cout << "size: " <<sizeof(int) << "|" << sizeof(float) << std::endl;*/
	////

	// Create one OpenGL texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

	// OpenGL has now copied the data. Free our own version
	delete [] data;


	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	*/
	// nastavime filtrovani

	//ostra textura i po roztahnuti
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); 

	// ... nice trilinear filtering.
	/*
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);*/
	glGenerateMipmap(GL_TEXTURE_2D);

	// Return the ID of the texture we just created
	return textureID;
}

int loadBMP_customData(const char * imagepath, BMPFile * bmpfile){

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	// Actual RGB data

	// Open the file
	FILE * file = fopen(imagepath,"rb");
	if (!file)							    {printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0;}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if ( fread(header, 1, 54, file)!=54 ){ 
		printf("Not a correct BMP file - 54 bytes\n");
		return 0;//TODO exception
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
	bmpfile->p_buffer = new unsigned char [imageSize];

	std::cout << "sizeof: " << sizeof(unsigned char) << "|" << sizeof(uint32_t) << std::endl;

	// Read the actual data from the file into the buffer
	fread(bmpfile->p_buffer,sizeof(unsigned char),imageSize,file);

	// Everything is in memory now, the file wan be closed
	fclose (file);

	bmpfile->n_height=height;
	bmpfile->n_width=width;
	//bmpfile->imageSize=imageSize;
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