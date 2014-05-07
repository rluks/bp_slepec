#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;//r-height,g-y velocity v,b-x velocity u
uniform sampler2D depthmap;

uniform vec3 pixel_size; // pixel_size.z = 0

const float delta_t = 0.01;
const float delta_x = 30.0/600.0;
const float delta_y = 30.0/600.0;

float c = 2.0;//dissipation const.

void main()
{
	//frag_color = texture(n_tex, v_texcoord);
	//frag_color.r = texture(n_tex, v_texcoord).r;
	//frag_color.rgb = texture(n_tex, v_texcoord).brg;
	//frag_color.rgb = texture(n_tex, v_texcoord).rgb;
	
	float depth = texture(depthmap, vec2(v_texcoord.x, 1 - v_texcoord.y)).r;

	//test
	//float depth = 1.0;

	if (depth <= 0.0)
	{
		frag_color.r = 0.0;
		frag_color.g = 0.0;
		frag_color.b = 0.0;
	}
	else
	{
		float dudx = texture(n_tex, v_texcoord.xy).b -
			texture(n_tex, v_texcoord.xy - vec2(pixel_size.x,0)).b;//velikostli

		float dvdy = texture(n_tex, v_texcoord.xy).g -
			texture(n_tex, v_texcoord.xy - vec2(0,pixel_size.y)).g;

		float h = texture(n_tex, v_texcoord.xy).r;

		float val = 0.5 - max(abs(v_texcoord.x - 0.5), abs(v_texcoord.y - 0.5));
		float k = 1.0 - smoothstep(.05, .01, val);

		h = h - delta_t*depth*((dudx)/delta_x + (dvdy)/delta_y);

		//if(h > 0.0 && h < 0.0005)
		//	h -= h/5.0;
		h = h * k;

		frag_color.r = h;
		frag_color.b = texture(n_tex, v_texcoord.xy).b;
		frag_color.g = texture(n_tex, v_texcoord.xy).g;
	}
	
}