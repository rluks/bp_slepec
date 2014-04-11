#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

uniform vec2 position;
uniform bool facing_left;

out vec2 v_texcoord;

void main()
{
	vec2 new = vec2(v_pos.x + position.x, v_pos.y + position.y);
 
	//prepocet ze souradneho systemu fyziky(0 horni levy roh) na zobrazeni(0 ve stredu)
	float s = (2.0 * new.x)/32.0 - 1.0;//TODO posilat velikost mapy 32
	float t = 1.0 - (2.0 * new.y)/24.0;//TODO posilat velikost mapy 24

	gl_Position = vec4(s, t, 0.0, 1.0);

	if(facing_left)
		v_texcoord.x = 1 - v_tex.x;
	else
		v_texcoord.x = v_tex.x;

	v_texcoord.y = v_tex.y;
}