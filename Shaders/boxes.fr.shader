#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

void main()
{
	frag_color =  vec4(1.0, 0.3, 0.6, 1.0);
}