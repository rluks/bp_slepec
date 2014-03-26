#version 330
in vec2 v_texcoord; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_tex;//r-height,g-y velocity v,b-x velocity u
uniform sampler2D depthmap;

uniform vec3 pixel_size; // pixel_size.z = 0

const float delta_t = 0.01;
const float delta_x = 30.0/800.0;//TODO posilat
const float delta_y = 30.0/600.0;

void main()
{
	//frag_color = texture(n_tex, v_texcoord);
	//frag_color.r = texture(n_tex, v_texcoord).r;
	//frag_color.rgb = texture(n_tex, v_texcoord).brg;
	//frag_color.rgb = texture(n_tex, v_texcoord).rgb;
	
	float depth = texture(depthmap, v_texcoord.xy).r;

	//test
	depth = 1.0;

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

		h = h - delta_t*depth*((dudx)/delta_x + (dvdy)/delta_y);

		frag_color.r = h;
		frag_color.b = texture(n_tex, v_texcoord.xy).b;
		frag_color.g = texture(n_tex, v_texcoord.xy).g;
	}
	
}