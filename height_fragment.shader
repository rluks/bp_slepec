// FRAGMENT PROGRAM

uniform samplerRect heightmap;
uniform samplerRect velocitymap;
uniform samplerRect depthmap;

const float delta_t = 0.01;
const float delta_x = 30.0/512.0;
const float delta_y = 30.0/512.0;

void main()
{
	float depth = textureRect(depthmap, gl_TexCoord[0].xy).r;

	if (depth <= 0.0)
	{
		gl_FragColor.r = 0.0;
		gl_FragColor.g = 0.0;
	}
	else
	{
		float dudx = textureRect(velocitymap, gl_TexCoord[0].xy).r -
		textureRect(velocitymap, gl_TexCoord[0].xy - vec2(1,0)).r;

		float dvdy = textureRect(velocitymap, gl_TexCoord[0].xy).g -
		textureRect(velocitymap, gl_TexCoord[0].xy - vec2(0,1)).g;

		float h = textureRect(heightmap, gl_TexCoord[0].xy).r;

		h = h - delta_t*depth*((dudx)/delta_x + (dvdy)/delta_y);

		gl_FragColor.r = h;
	}
}