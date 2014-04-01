#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

out vec2 v_texcoord;

void main()
{
	gl_Position = vec4(v_pos.xy, -0.1, 1.0);
	gl_PointSize = 5.0;
}