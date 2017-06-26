#version 120

attribute vec3		v_pos;
attribute vec2		v_uv;

uniform vec2		position;
uniform vec2		size;

varying vec2		tex_coord;

void main()
{
	gl_Position.xyz = vec3(position.x + (v_pos.x * size.x),
		position.y - (v_pos.y * size.y), 0.0);
	// gl_Position.xyz = v_pos;
	gl_Position.w = 1.0;
	tex_coord = v_uv;
	// if (v_pos.x > 0.0)
	// 	tex_coord.x = 1.0f;
	// if (v_pos.y > 0.0)
	// 	tex_coord.y = 1.0f;
}
