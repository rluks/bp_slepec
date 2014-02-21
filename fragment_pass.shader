#version 330
in vec2 v_texcoord; // vstup z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_box_tex;

void main()
{
	frag_color = texture(n_box_tex, v_texcoord).r * vec4(0.4, 1.0, 0.8, 1.0);
}