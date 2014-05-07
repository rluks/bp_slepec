//potlacuje warning C4996: 'fopen' was declared deprecated:
#define _CRT_SECURE_NO_DEPRECATE

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

#include <string>
#include <iostream>

#include "Transform.h"

#include <glm/vec2.hpp> 
#include <glm/vec3.hpp> 

//pomocne funkce
std::string getTextFileString(std::string);
bool CheckShader(GLuint, const char *);
bool CheckProgram(GLuint, const char *);

/**
 *	@brief prace s shadery
 */
struct Shader {
protected:

	std::string vertexpath;
	std::string fragmentpath;

public:

	bool compile(GLuint *, GLuint *, GLuint *);
	void deleteShader(GLuint *, GLuint *, GLuint *);
};

/**
 *	@brief testovaci shader na rozmazavani
 */
struct ShaderBlur : public Shader {
public:
	GLuint n_vertex_shader, n_fragment_shader, n_program_object;
	GLuint n_texture_sampler_uniform, pixel_size;
	bool compile();
	void bind(int, int);
	void deleteShader();
};

/**
 *	@brief vykresluje texturu vlneni
 */
struct ShaderPass : public Shader {
public:
	GLuint n_vertex_shader_pass, n_fragment_shader_pass, n_program_object_pass;
	GLuint n_mvp_matrix_uniform_pass, 
	n_texture_sampler_uniform_pass,
	n_depthmap_sampler_uniform_pass,
	color_uniform_pass;
	bool compile();
	void bind(Matrix4f, glm::vec3);
	void deleteShader();
};

/**
 *	@brief vykresluje herni postavu
 */
struct ShaderHero : public Shader {
public:
	GLuint n_vertex_shader_hero, n_fragment_shader_hero, n_program_object_hero;
	GLuint n_texture_sampler_uniform_hero,
	n_depthmap_sampler_uniform_hero,
	position_vector,
	facing_left_uniform_hero;
	bool compile();
	void bind(glm::vec2, bool);
	void deleteShader();
};

/**
 *	@brief pocita simulaci vlneni - rychlost vln
 */
struct ShaderVelocity : public Shader {
public:
	GLuint n_vertex_shader_velocity, n_fragment_shader_velocity, n_program_object_velocity;
	GLuint n_texture_sampler_uniform_velocity, n_depthmap_sampler_uniform, pixel_size;
	bool compile();
	void bind(int, int);
	void deleteShader();
};

/**
 *	@brief pocita simulaci vlneni - vysku hladiny
 */
struct ShaderHeight : public Shader {
public:
	GLuint n_vertex_shader_height, n_fragment_shader_height, n_program_object_height;
	GLuint n_texture_sampler_uniform_height, n_depthmap_sampler_uniform_height, pixel_size;
	bool compile();
	void bind(int, int);
	void deleteShader();
};

/**
 *	@brief pridani "kapky" zdroje zvuku
 */
struct ShaderPoint : public Shader {
public:
	GLuint n_vertex_shader_point, n_fragment_shader_point, n_program_object_point;
	bool compile();
	void bind();
	void deleteShader();
};

/**
 *	@brief testovaci shader na vykresleni bodu kolize
 */
struct ShaderPointSimple : public Shader {
public:
	GLuint n_vertex_shader_pointSimple, n_fragment_shader_pointSimple, n_program_object_pointSimple;
	bool compile();
	void bind();
	void deleteShader();
};

/**
 *	@brief testovaci shader na vykresleni geometrie
 */
struct ShaderBoxes : public Shader {
public:
	GLuint n_vertex_shader_boxes, n_fragment_shader_boxes, n_program_object_boxes;
	GLuint color_unifrom_boxes;
	bool compile();
	void bind(glm::vec3);
	void deleteShader();
};

/**
 *	@brief vykresluje hmatovy vjem
 */
struct ShaderSurface : public Shader {
public:
	GLuint n_vertex_shader_curve, n_fragment_shader_curve, n_program_object_curve;
	GLuint point1_uniform_curve, point2_uniform_curve, amplitude_uniform_curve, frequency_uniform_curve, phase_uniform_curve, color_uniform_curve;
	bool compile(std::string, std::string);
	void bind(glm::vec2, glm::vec2, float, float, float, glm::vec3);
	void deleteShader();
};

#endif