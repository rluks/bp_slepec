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
	//prepocet ze souradneho systemu fyziky(0 horni levy roh) na zobrazeni(0 ve stredu)
	float s1 = (2.0 * point1.x)/32.0 - 1.0;//TODO posilat velikost mapy 32
	float t1 = 1.0 - (2.0 * point1.y)/24.0;//TODO posilat velikost mapy 24

	float s2 = (2.0 * point2.x)/32.0 - 1.0;//TODO posilat velikost mapy 32
	float t2 = 1.0 - (2.0 * point2.y)/24.0;//TODO posilat velikost mapy 24

	float s = s1 + (s2-s1) * v_pos.x;
	float t = t1 + (t2-t1) * v_pos.y;
	
	//t+=mod(v_pos.x, 0.2);	

	//y(t)=A*sin(2pi*f*t + phase)
	//t = [0, 1]
	t += amplitude * sin(6.28 * frequency * v_pos.x + phase);

	gl_Position = vec4(s, t, 0.0, 1.0);
	//gl_PointSize = 2.0;
	v_texcoord = v_tex;
}