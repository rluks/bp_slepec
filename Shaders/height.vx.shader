#version 330
in vec2 v_tex;
in vec3 v_pos;

out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(v_pos, 1.0);
	//gl_TexCoord[0] = gl_MultiTexCoord0;
	v_texcoord = v_tex;
}