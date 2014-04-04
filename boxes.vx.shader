#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu
out vec2 v_texcoord;

void main()
{

 
	//prepocet ze souradneho systemu fyziky(0 horni levy roh) na zobrazeni(0 ve stredu)
	float s = (2.0 * v_pos.x)/32.0 - 1.0;//TODO posilat velikost mapy 32
	float t = 1.0 - (2.0 * v_pos.y)/24.0;//TODO posilat velikost mapy 24

	vec3 new_pos = vec3(s, t, 0.0);

	gl_Position = vec4(new_pos.xy, -0.1, 1.0);

	v_texcoord = v_tex;
}