#version 330
in vec2 v_texcoord;
out vec4 frag_color;

uniform sampler2D n_tex;//r-height,g-y velocity v,b-x velocity u
uniform sampler2D depthmap;

uniform vec3 pixel_size; // pixel_size.z = 0

const float delta_t = 0.01;

const float delta_x = 30.0/600.0;
const float delta_y = 30.0/600.0;



const float g = 1.0;
float c = 2.0;//dissipation const.

void main()
{


	float depth = texture(depthmap, vec2(v_texcoord.x, 1 - v_texcoord.y)).r +
		texture(depthmap, vec2(v_texcoord.x, (1 - v_texcoord.y) + pixel_size.y)).r +
		texture(depthmap, vec2(v_texcoord.x, (1 - v_texcoord.y) -pixel_size.y)).r +
		texture(depthmap, vec2(v_texcoord.x + pixel_size.x, 1 - v_texcoord.y)).r +
		texture(depthmap, vec2(v_texcoord.x - pixel_size.x, 1 - v_texcoord.y)).r;

	depth /= 5.0;

	//test
	//float depth = 1.0;

	if(depth <= 0.0)
	{	
		frag_color.r = 0.0;
		frag_color.b = 0.0;
		frag_color.g = 0.0;
	}
	else
	{
		// Height
		float dhdx = texture(n_tex, v_texcoord.xy + vec2(pixel_size.x,0)).r -
			texture(n_tex, v_texcoord.xy).r;

		float dhdy = texture(n_tex, v_texcoord.xy + vec2(0,pixel_size.y)).r -
			texture(n_tex, v_texcoord.xy).r;

		// Velocity
		float dudxx = texture(n_tex, v_texcoord.xy + vec2(pixel_size.x,0)).b -
			2.0 * texture(n_tex, v_texcoord.xy).b +
			texture(n_tex, v_texcoord.xy - vec2(pixel_size.x,0)).b;

		float dvdyy = texture(n_tex, v_texcoord.xy + vec2(0,pixel_size.y)).g -
			2.0 * texture(n_tex, v_texcoord.xy).g +
			texture(n_tex, v_texcoord.xy - vec2(0,pixel_size.y)).g;

		float hx = 0.0;
		float hy = 0.0;

		float val = 0.5 - max(abs(v_texcoord.x - 0.5), abs(v_texcoord.y - 0.5));
		float k = 1.0 - smoothstep(.05, .1, val);
		float c1 = c + 15.0 * k;

		hx += (delta_t/delta_x) * (c1*delta_t*dudxx - g*dhdx);
		hy += (delta_t/delta_y) * (c1*delta_t*dvdyy - g*dhdy);

		float next_u = texture(n_tex, v_texcoord.xy).b;
		float next_v = texture(n_tex, v_texcoord.xy).g;

		next_u += hx;
		next_v += hy;

		frag_color.b = next_u;
		frag_color.g = next_v;
		frag_color.r = texture(n_tex, v_texcoord).r;
	}
	
}