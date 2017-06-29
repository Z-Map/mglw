#version 120

uniform vec4		color;
uniform vec4		border_color;

uniform vec4		borders;

varying vec2		tex_coord;

void main()
{
	vec4			mv;
	// vec4			bv;

	gl_FragColor = color;
	mv.xy = tex_coord;
	mv = vec4(-borders.xy, 1.0f - borders.zw) + vec4(tex_coord, -tex_coord);
	// bv = 1.0f - borders;
	if ((mv.x <= 0.0f) || (mv.z <= 0.0f)
		|| (mv.y <= 0.0f) || (mv.w <= 0.0f))
		gl_FragColor = border_color;
	// gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

	// gl_FragColor = texture2D(image, tex_coord);
}
