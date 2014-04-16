#version 330
in vec2 v_tex;
in vec3 v_pos;

out vec2 v_texcoord;

uniform vec2 point1;
uniform vec2 point2;

uniform float amplitude;
uniform float frequency;
uniform float phase;

void main()
{
	float s = point1.x + (point2.x-point1.x) * v_pos.x;
	float t = point1.y + (point2.y-point1.y) * v_pos.y;

	float t_new = t + amplitude * sin(6.28 * frequency * v_pos.x + phase);
	t_new += amplitude;

	//prepocet ze souradneho systemu fyziky(0 horni levy roh) na zobrazeni(0 ve stredu)
	float s0 = (2.0 * s)/32.0 - 1.0;//TODO posilat velikost mapy 32
	float t0 = 1.0 - (2.0 * t_new)/24.0;//TODO posilat velikost mapy 24

	gl_Position = vec4(s0, t0, 0.0, 1.0);
	v_texcoord = v_tex;
}