#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

uniform float f_time; // parametr shaderu - cas pro animovani

uniform mat4 t_modelview_projection_matrix; // parametr shaderu - transformacni matice

out vec2 v_texcoord;
out vec2 v_texcoord1;
out vec2 v_texcoord2;
out vec2 v_texcoord3; // vystupy - souradnice textury

void main()
{
	gl_Position = t_modelview_projection_matrix * vec4(v_pos, 1.0); // musime zapsat pozici
	v_texcoord = v_tex; // zkopirujeme souradnice textury pro fragment shader
	v_texcoord1 = v_tex + vec2(sin(f_time), f_time);
	v_texcoord2 = v_tex + vec2(0.0, f_time * 1.5 + 1.0);
	v_texcoord3 = v_tex + vec2(sin(f_time + 1.0), f_time + 2.0); // spocitame dalsi souradnice textury pro animaci ohne
}