#include "Shader.h"

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

bool Shader::compile(GLuint * n_vertex_shader, GLuint * n_fragment_shader, GLuint * n_program_object){
	std::string vertexString = getTextFileString(vertexpath);
	const char *p_s_vertex_shader = vertexString.c_str();

	std::string fragmentString = getTextFileString(fragmentpath);
	const char *p_s_fragment_shader = fragmentString.c_str();

	*n_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(*n_vertex_shader, 1, &p_s_vertex_shader, NULL);
	glCompileShader(*n_vertex_shader);
	if(!CheckShader(*n_vertex_shader, "vertex shader"))
		return false;

	*n_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(*n_fragment_shader, 1, &p_s_fragment_shader, NULL);
	glCompileShader(*n_fragment_shader);
	if(!CheckShader(*n_fragment_shader, "fragment shader"))
		return false;
	*n_program_object = glCreateProgram();

	glAttachShader(*n_program_object, *n_vertex_shader);
	glAttachShader(*n_program_object, *n_fragment_shader);
	// zkompiluje vertex / fragment shader, pripoji je k programu

	glBindAttribLocation(*n_program_object, 0, "v_tex");//2hodnoty vec2
	glBindAttribLocation(*n_program_object, 1, "v_pos");//3hodnoty vec3
	// nabinduje atributy (propojeni mezi obsahem VBO a vstupem do vertex shaderu)

	glBindFragDataLocation(*n_program_object, 0, "frag_color");
	// nabinduje vystupni promenou (propojeni mezi framebufferem a vystupem fragment shaderu)

	glLinkProgram(*n_program_object);
	if(!CheckProgram(*n_program_object, "program"))
		return false;
	// slinkuje program

	return true;
}

bool ShaderBlur::compile(){
	vertexpath  = "blur.vx.shader";
	fragmentpath = "blur.fr.shader";

	if(!Shader::compile(&n_vertex_shader, &n_fragment_shader, &n_program_object))
		return false;//TODO asi spis vyjimka

	//n_mvp_matrix_uniform = glGetUniformLocation(n_program_object, "t_modelview_projection_matrix");
	n_texture_sampler_uniform = glGetUniformLocation(n_program_object, "n_tex");
	//n_time_uniform = glGetUniformLocation(n_program_object, "f_time");
	pixel_size = glGetUniformLocation(n_program_object, "pixel_size");

	return true;
}

void ShaderBlur::bind(int width, int height){
	glUseProgram(n_program_object);
	{
		glUniform1i(n_texture_sampler_uniform, 0);
		glUniform3f(pixel_size, 1.0f/width, 1.0f/height, 0.0f);
	}
}

//pass
bool ShaderPass::compile(){
	vertexpath  = "pass.vx.shader";
	fragmentpath = "pass.fr.shader";
	if(!Shader::compile(&n_vertex_shader_pass, &n_fragment_shader_pass, &n_program_object_pass))
		return false;

	n_mvp_matrix_uniform_pass = glGetUniformLocation(n_program_object_pass, "t_modelview_projection_matrix");
	n_texture_sampler_uniform_pass = glGetUniformLocation(n_program_object_pass, "n_tex");

	return true;
}

void ShaderPass::bind(Matrix4f t_mvp){
	glUseProgram(n_program_object_pass);
	{
		glUniformMatrix4fv(n_mvp_matrix_uniform_pass, 1, GL_FALSE, &t_mvp[0][0]);
		glUniform1i(n_texture_sampler_uniform_pass, 0);
	}
}

//velocity
bool ShaderVelocity::compile(){
	vertexpath  = "velocity.vx.shader";
	fragmentpath = "velocity.fr.shader";
	if(!Shader::compile(&n_vertex_shader_velocity, &n_fragment_shader_velocity, &n_program_object_velocity))
		return false;

	n_texture_sampler_uniform_velocity = glGetUniformLocation(n_program_object_velocity, "n_tex");//r-height,g-y velocity,b-x velocity
	n_depthmap_sampler_uniform = glGetUniformLocation(n_program_object_velocity, "depthmap");
	pixel_size = glGetUniformLocation(n_program_object_velocity, "pixel_size");

	return true;
}

void ShaderVelocity::bind(int width, int height){
	glUseProgram(n_program_object_velocity);
	{
		glUniform1i(n_texture_sampler_uniform_velocity, 0);
		glUniform1i(n_depthmap_sampler_uniform, 1);
		glUniform3f(pixel_size, 1.0f/width, 1.0f/height, 0.0f);
	}
}

//height
bool ShaderHeight::compile(){
	vertexpath  = "height.vx.shader";
	fragmentpath = "height.fr.shader";
	if(!Shader::compile(&n_vertex_shader_height, &n_fragment_shader_height, &n_program_object_height))
		return false;

	n_texture_sampler_uniform_height = glGetUniformLocation(n_program_object_height, "n_tex");//r-height,g-y velocity v,b-x velocity u
	n_depthmap_sampler_uniform_height = glGetUniformLocation(n_program_object_height, "depthmap");
	pixel_size = glGetUniformLocation(n_program_object_height, "pixel_size");

	return true;
}

void ShaderHeight::bind(int width, int height){
	glUseProgram(n_program_object_height);
	{
		glUniform1i(n_texture_sampler_uniform_height, 0);
		glUniform1i(n_depthmap_sampler_uniform_height, 1);
		glUniform3f(pixel_size, 1.0f/width, 1.0f/height, 0.0f);
	}
}
