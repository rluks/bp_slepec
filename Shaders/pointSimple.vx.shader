#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

uniform vec2 map_size;

out vec2 v_texcoord;

void main()
{
	//prepocet ze souradneho systemu fyziky(0 horni levy roh) na zobrazeni(0 ve stredu)
	float s = (2.0 * v_pos.x)/map_size.x - 1.0;
	float t = 1.0 - (2.0 * v_pos.y)/map_size.y;

	gl_Position = vec4(s, t, 0.0, 1.0);
	gl_PointSize = 5.0;
}