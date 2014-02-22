#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

uniform float f_time; // parametr shaderu - cas pro animovani

uniform mat4 t_modelview_projection_matrix; // parametr shaderu - transformacni matice

out vec2 v_texcoord;

void main()
{
	gl_Position = /*t_modelview_projection_matrix * */vec4(v_pos, 1.0); // musime zapsat pozici
	v_texcoord = v_tex; // zkopirujeme souradnice textury pro fragment shader
}