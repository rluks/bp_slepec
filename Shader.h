#include <GL/glew.h>
#include <string>
#include <iostream>

//pomocne funkce
std::string getTextFileString(std::string);
bool CheckShader(GLuint, const char *);
bool CheckProgram(GLuint, const char *);

struct Shader {
protected:

	std::string vertexpath;
	std::string fragmentpath;

	GLuint n_vertex_shader, n_fragment_shader, n_program_object;

public:
	bool compile();
};

struct ShaderBlur : public Shader {
public:
	GLuint n_mvp_matrix_uniform, n_texture_sampler_uniform, n_time_uniform, pixel_size;
	bool compile();
	void bind();
	void use(int, int);
};