#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;

uniform vec3 pixel_size; // pixel_size.z = 0

void main()
{
	float temp;
	temp =  texture(n_tex, v_texcoord).r	+ texture(n_tex, v_texcoord+vec2(pixel_size.z, pixel_size.y)).r
												+ texture(n_tex, v_texcoord+vec2(-pixel_size.x, pixel_size.z)).r
												+ texture(n_tex, v_texcoord+vec2(pixel_size.z, -pixel_size.y)).r
												+ texture(n_tex, v_texcoord+vec2(pixel_size.x, pixel_size.z)).r;
	temp = temp/5.0;

	frag_color = temp * vec4(1.0, 1.0, 1.0, 1.0);
}
