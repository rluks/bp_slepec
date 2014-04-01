#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;

//uniform velikostpixelu

void main()
{
	float height = 30* texture(n_tex, v_texcoord).r;
	frag_color = vec4(height, -height, 0.0, 1.0);

	frag_color = 50 * texture(n_tex, v_texcoord).r * vec4(0.4, 1.0, 0.8, 1.0);

	/*if(height == 0.0)
		frag_color = vec4(0.5, 0.5, 0.5, 1.0);*/
	
	/*
	if(height >= 0.0 && height < 0.1)
		frag_color = vec4(1.0, 0.0, 0.0, 1.0);
	else if(height >= 0.1 && height < 0.2)
		frag_color = vec4(0.0, 1.0, 0.0, 1.0);
	else if(height >= 0.9)
		frag_color = vec4(0.0, 0.0, 1.0, 1.0);
	else
		frag_color = texture(n_tex, v_texcoord).r * vec4(0.4, 1.0, 0.8, 1.0);*/

	
	/*
	if(height < 0.0)
		frag_color = height * vec4(0.4, 0.0, 0.0, 1.0);
	else if(height == 0.0)
		frag_color = vec4(0.5, 0.5, 0.5, 1.0);
	else
		frag_color = height * vec4(0.0, 0.4, 0.0, 1.0);*/
}