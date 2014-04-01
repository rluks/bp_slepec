#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;

//uniform velikostpixelu

float c = 2.0;//dissipation const.

void main()
{

	float height = 50 * texture(n_tex, v_texcoord).r;
	height = abs(smoothstep(.01, 1.0, height));
	frag_color = height * vec4(0.4, 1.0, 0.8, 1.0);

	//float height = 50* texture(n_tex, v_texcoord).r;
	//frag_color = vec4(height, -height, 0.0, 1.0);

	/*if(height == 0.0)//prekazky
		frag_color = vec4(0.5, 0.5, 0.5, 1.0);*/
}