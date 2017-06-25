#version 120

uniform vec4		color;
uniform sampler2D	chartex;

varying vec2		tex_coord;

void main()
{
	gl_FragColor = vec4(1.0,1.0,1.0,1.0);
	gl_FragColor.a *= texture2D(chartex, tex_coord).r;
}
