#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_box_tex;

uniform float pixel_size_x_value, pixel_size_y_value;

void main()
{
	float temp;
	temp =  texture(n_box_tex, v_texcoord).r	+ texture(n_box_tex, v_texcoord+vec2(0, pixel_size_y_value)).r
												+ texture(n_box_tex, v_texcoord+vec2(pixel_size_x_value, pixel_size_y_value)).r
												+ texture(n_box_tex, v_texcoord+vec2(pixel_size_x_value, 0)).r;
	temp = temp/4.0;

	frag_color = temp * vec4(1.0, 1.0, 1.0, 1.0);
}
