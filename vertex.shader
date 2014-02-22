#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(v_pos, 1.0); // musime zapsat pozici
	v_texcoord = v_tex; // zkopirujeme souradnice textury pro fragment shader
}