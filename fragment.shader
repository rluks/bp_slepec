#version 330
in vec2 v_texcoord, v_texcoord1, v_texcoord2, v_texcoord3; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_box_tex, n_fire_tex;

void main()
{
	frag_color = texture(n_box_tex, v_texcoord).r * vec4(0.4, 1.0, 0.8, 1.0);
	//frag_color = vec4(0.0, 1.0, 0.0, 0.0);
}