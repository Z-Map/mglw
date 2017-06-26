#version 120

uniform vec4		color;
uniform vec4		border_color;
uniform vec4		shadow_color;

varying vec2 tex_coord;

void main()
{
	gl_FragColor = texture2D(image, tex_coord);
}
