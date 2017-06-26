#version 120

uniform vec4		color;
uniform sampler2D	chartex;

varying vec2		tex_coord;

void main()
{
	gl_FragColor = color;
	gl_FragColor.a *= texture2D(chartex, tex_coord).r;
}
