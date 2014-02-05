#version 330
in vec2 v_texcoord, v_texcoord1, v_texcoord2, v_texcoord3; // vstupy z vertex shaderu
out vec4 frag_color; // vystup do framebufferu

uniform sampler2D n_box_tex, n_fire_tex;

void main()
{
	vec4 box_color = texture2D(n_box_tex, v_texcoord); // precte texturu krabice
	
	// precte tri vrstvy textury ohne (animace vznika vypoctem souradnic textury ve vertex shaderu)
	vec4 fire_color = texture2D(n_fire_tex, v_texcoord1) +	texture2D(n_fire_tex, v_texcoord2) + texture2D(n_fire_tex, v_texcoord3);
	
	frag_color = mix(fire_color, box_color, box_color.a); // smicha texturu a krabici podle alfa kanalu krabice (je tam vyznacene co ma byt pruhledne a co ne)
}