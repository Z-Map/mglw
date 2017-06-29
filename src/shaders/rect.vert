#version 120

attribute vec2	v_pos;

uniform vec2	position;
uniform vec2	size;

varying vec2	tex_coord;

void main()
{
	gl_Position.xyz = vec3(position + (v_pos * size), 0.0);
	gl_Position.w = 1.0;
	tex_coord = v_pos;
}
