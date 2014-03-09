// FRAGMENT PROGRAM

uniform samplerRect heightmap;
uniform samplerRect velocitymap;
uniform samplerRect depthmap;

const float delta_t = 0.01;
const float delta_x = 30.0/512.0;
const float delta_y = 30.0/512.0;

const float g = 1.0;
const float c = 2.0;

void main()
{
	float depth = textureRect(depthmap, gl_TexCoord[0].xy).r +
		textureRect(depthmap, gl_TexCoord[0].xy+vec2(0,1)).r +
		textureRect(depthmap, gl_TexCoord[0].xy+vec2(1,1)).r +
		textureRect(depthmap, gl_TexCoord[0].xy+vec2(1,0)).r;

	depth /= 4.0;

	if (depth <= 0.0)
	{
		gl_FragColor.r = 0.0;
		gl_FragColor.g = 0.0;
	}
	else
	{
		// Height
		float dhdx = textureRect(heightmap, gl_TexCoord[0].xy + vec2(1,0)).r -
			textureRect(height map, gl_TexCoord[0].xy).r;

		float dhdy = textureRect(heightmap, gl_TexCoord[0].xy + vec2(0,1)).r -
			textureRect(height map, gl_TexCoord[0].xy).r;

		float dudxx = textureRect(velocitymap, gl_TexCoord[0].xy + vec2(1,0)).r -
			2.0 * textureRect(velocitymap, gl_TexCoord[0].xy).r +
			textureRect(velocitymap, gl_TexCoord[0].xy - vec2(1,0)).r;

		float dvdyy = textureRect(velocitymap, gl_TexCoord[0].xy + vec2(0,1)).g -
			2.0 * textureRect(velocitymap, gl_TexCoord[0].xy).g +
			textureRect(velocitymap, gl_TexCoord[0].xy - vec2(0,1)).g;

		float hx = 0.0;
		float hy = 0.0;

		hx += (delta_t/delta_x) * (c*delta_t*dudxx-g*dhdx);
		hy += (delta_t/delta_y) * (c*delta_t*dvdyy-g*dhdy);

		float next_u = textureRect(velocitymap, gl_TexCoord[0].xy).r;
		float next_v = textureRect(velocitymap, gl_TexCoord[0].xy).g;

		next_u += hx;
		next_v += hy;

		gl_FragColor.r = next_u;
		gl_FragColor.g = next_v;
	}
}