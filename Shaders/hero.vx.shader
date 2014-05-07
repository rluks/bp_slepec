#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

uniform vec2 position;
uniform bool facing_left;
uniform vec2 map_size;

out vec2 v_texcoord;

void main()
{
	vec2 new = vec2(v_pos.x + position.x, v_pos.y + position.y);
 
	//prepocet ze souradneho systemu fyziky(0 horni levy roh) na zobrazeni(0 ve stredu)
	float s = (2.0 * new.x)/map_size.x - 1.0;
	float t = 1.0 - (2.0 * new.y)/map_size.y;

	gl_Position = vec4(s, t, 0.0, 1.0);

	if(facing_left)
		v_texcoord.x = 1 - v_tex.x;
	else
		v_texcoord.x = v_tex.x;

	v_texcoord.y = v_tex.y;
}