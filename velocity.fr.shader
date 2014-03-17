#version 330
in vec2 v_texcoord;
out vec4 frag_color;

uniform sampler2D n_tex;//r-height,g-y velocity v,b-x velocity u
uniform sampler2D depthmap;

const float delta_t = 0.01;
const float delta_x = 30.0/800.0;//TODO posilat
const float delta_y = 30.0/600.0;

const float g = 1.0;
const float c = 2.0;//dissipation const.

void main()
{
	//tohle se chova ocekavane, nahodna data se meni  - zrneni
	//frag_color.rgb = texture(n_tex, v_texcoord).brg;

	//s depthmap jsou problemy
	frag_color.r = texture(depthmap, v_texcoord).r;//tohle se zda nic nedela, stejne se vykresli nahodna data z tex
	frag_color.r = texture(depthmap, v_texcoord).g;//depthmap ma jen r kanal, proc tohle neni error?

	/*
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

		// Velocity
		float dudxx = texture(n_tex, v_texcoord.xy + vec2(1,0)).b -
			2.0 * texture(n_tex, v_texcoord.xy).b +
			texture(n_tex, v_texcoord.xy - vec2(1,0)).b;

		float dvdyy = texture(n_tex, v_texcoord.xy + vec2(0,1)).g -
			2.0 * texture(n_tex, v_texcoord.xy).g +
			texture(n_tex, v_texcoord.xy - vec2(0,1)).g;

		float hx = 0.0;
		float hy = 0.0;

		hx += (delta_t/delta_x) * (c*delta_t*dudxx-g*dhdx);
		hy += (delta_t/delta_y) * (c*delta_t*dvdyy-g*dhdy);

		float next_u = texture(n_tex, v_texcoord.xy).b;
		float next_v = texture(n_tex, v_texcoord.xy).g;

		next_u += hx;
		next_v += hy;

		frag_color.r = next_u;
		frag_color.g = next_v;
	}
	*/
}