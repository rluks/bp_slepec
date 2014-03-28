#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

//uniform sampler2D n_tex;

void main()
{
	vec2 texcoord = gl_PointCoord;
	float x = 1.0 - 2.0 * length(vec2(0.5, 0.5) - gl_PointCoord.xy);
	frag_color = vec4(max(x, 0.0), 0.0, 0.0, 0.0);
}