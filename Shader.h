#include <GL/glew.h>
#include <string>
#include <iostream>

#include "Transform.h"

//pomocne funkce
std::string getTextFileString(std::string);
bool CheckShader(GLuint, const char *);
bool CheckProgram(GLuint, const char *);

struct Shader {
protected:

	std::string vertexpath;
	std::string fragmentpath;

public:
	bool compile(GLuint *, GLuint *, GLuint *);
};

struct ShaderBlur : public Shader {
public:
	GLuint n_vertex_shader, n_fragment_shader, n_program_object;
	GLuint n_mvp_matrix_uniform, n_texture_sampler_uniform, n_time_uniform, pixel_size;
	bool compile();
	void bind();
	void use(int, int);
};

struct ShaderPass : public Shader {
public:
	GLuint n_vertex_shader_pass, n_fragment_shader_pass, n_program_object_pass;
	GLuint n_mvp_matrix_uniform_pass, 
	n_texture_sampler_uniform_pass;
	//n_time_uniform_pass;
	bool compile();
	void bind();
	void use(Matrix4f);
};

struct ShaderVelocity : public Shader {
public:
	GLuint n_vertex_shader_velocity, n_fragment_shader_velocity, n_program_object_velocity;
	GLuint n_texture_sampler_uniform_velocity, n_velocitymap_sampler_uniform;
	bool compile();
	void bind();
	void use();
};