#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_box_tex;

//uniform velikostpixelu

void main()
{
	float temp;
	temp =  texture(n_box_tex, v_texcoord).r	+ texture(n_box_tex, v_texcoord+vec2(0,0.01)).r
												+ texture(n_box_tex, v_texcoord+vec2(0.01,0.01)).r
												+ texture(n_box_tex, v_texcoord+vec2(0.01,0)).r;
	temp = temp/4.0;

	frag_color = temp * vec4(1.0, 1.0, 1.0, 1.0);
}
