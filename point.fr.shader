#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

//uniform sampler2D n_tex;

void main()
{
	vec2 texcoord = gl_PointCoord;
	frag_color.rgb = vec3(0.5, 0.5, 0.5);
}