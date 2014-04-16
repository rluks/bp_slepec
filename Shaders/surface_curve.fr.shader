#version 330
in vec2 v_texcoord;
out vec4 frag_color;

uniform vec3 color;

void main()
{
	vec2 texcoord = gl_PointCoord;
	frag_color =  vec4(color, 0.0);
}