#version 330
in vec2 v_texcoord;
out vec4 frag_color;

//uniform sampler2D heightmap;
uniform sampler2D n_tex;
uniform sampler2D velocitymap;
uniform sampler2D depthmap;

const float delta_t = 0.01;
const float delta_x = 30.0/800.0;//TODO posilat
const float delta_y = 30.0/600.0;

const float g = 1.0;
const float c = 2.0;//dissipation const.

void main()
{
	float depth = texture(depthmap, v_texcoord.xy).r +
		texture(depthmap, v_texcoord.xy+vec2(0,1)).r +
		texture(depthmap, v_texcoord.xy+vec2(1,1)).r +
		texture(depthmap, v_texcoord.xy+vec2(1,0)).r;

	depth /= 4.0;

	if(depth <= 0.0)
	{
		frag_color.r = 0.0;
		frag_color.g = 0.0;
	}
	else
	{
		// Height
		float dhdx = texture(n_tex, v_texcoord.xy + vec2(1,0)).r -
			texture(n_tex, v_texcoord.xy).r;

		float dhdy = texture(n_tex, v_texcoord.xy + vec2(0,1)).r -
			texture(n_tex, v_texcoord.xy).r;

		float dudxx = texture(velocitymap, v_texcoord.xy + vec2(1,0)).r -
			2.0 * texture(velocitymap, v_texcoord.xy).r +
			texture(velocitymap, v_texcoord.xy - vec2(1,0)).r;

		float dvdyy = texture(velocitymap, v_texcoord.xy + vec2(0,1)).g -
			2.0 * texture(velocitymap, v_texcoord.xy).g +
			texture(velocitymap, v_texcoord.xy - vec2(0,1)).g;

		float hx = 0.0;
		float hy = 0.0;

		hx += (delta_t/delta_x) * (c*delta_t*dudxx-g*dhdx);
		hy += (delta_t/delta_y) * (c*delta_t*dvdyy-g*dhdy);

		float next_u = texture(velocitymap, v_texcoord.xy).r;
		float next_v = texture(velocitymap, v_texcoord.xy).g;

		next_u += hx;
		next_v += hy;

		frag_color.r = next_u;
		frag_color.g = next_v;
	}
}