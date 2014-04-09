#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;
uniform sampler2D depthmap;

void main()
{
	float height = abs(50 * texture(n_tex, v_texcoord).r);
	height = smoothstep(.01, 1.0, height);
	frag_color = height * vec4(0.4, 1.0, 0.8, 1.0);
	
	float depth = texture(depthmap, vec2(v_texcoord.x, 1 - v_texcoord.y)).r;
	frag_color.g = 1-depth;
}