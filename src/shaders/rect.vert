#version 120

attribute vec2	v_pos;
attribute vec2	v_uv;

uniform vec2	screen;
uniform vec2	position;
uniform vec2	size;

varying vec2	tex_coord;

void main()
{
	gl_Position.xyz = vec3(v_pos, 0.0);
	gl_Position.w = 1.0;
	tex_coord = v_uv;
}
