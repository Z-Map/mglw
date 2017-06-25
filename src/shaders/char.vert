#version 120

attribute vec3		v_pos;
attribute vec2		v_uv;

uniform vec2		screen;
uniform vec2		position;
uniform float		size;

varying vec2		tex_coord;

void main()
{
	vec2 fsize;
	vec2 fpos;
	fsize = vec2(size / screen.x, size / screen.y);
	fpos = position / screen;
	gl_Position.xyz = vec3(fpos.x + (v_pos.x * fsize.x),
		fpos.y - (v_pos.y * fsize.y), 0.0);
	// gl_Position.xyz = v_pos;
	gl_Position.w = 1.0;
	tex_coord = v_uv;
	// if (v_pos.x > 0.0)
	// 	tex_coord.x = 1.0f;
	// if (v_pos.y > 0.0)
	// 	tex_coord.y = 1.0f;
}
