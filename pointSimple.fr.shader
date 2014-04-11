#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

void main()
{
	vec2 texcoord = gl_PointCoord;
	frag_color = vec4(0.0, 0.9, 0.0, 0.0);
}