#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;
uniform sampler2D depthmap;

void main()
{
	frag_color =  texture(n_tex, v_texcoord.xy);
}