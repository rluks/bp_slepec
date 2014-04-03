#version 330
in vec2 v_tex;
in vec3 v_pos; // atributy - vstup z dat vrcholu

uniform mat4 t_modelview_projection_matrix; // parametr shaderu - transformacni matice
uniform vec2 movement;

out vec2 v_texcoord;

void main()
{
	vec3 new_pos = v_pos + vec3(movement.xy, 0.0);
	gl_Position = vec4(new_pos.xy, -0.1, 1.0);

	v_texcoord = v_tex;
}