/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 17:30:03 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/29 18:51:03 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glload/gl_all.h"
#include "mgl/ressources/shaders/basic.h"
#include "mgl/shaders.h"
#include "mgl/drawer.h"
# include "mgl/drawer.h"

static int			rect_init(GLuint *vbo, mglsha *shr)
{
	float			vertx[] = {
		// central rect
		0.5f, 0.5f,
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, 0.0f
	};

	*shr = mglrect2d_shader;
	mgl_compile_shader(shr);
	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), vertx, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (1);
}

v4f				coltov4f(unsigned int color)
{
	unsigned char	*col;

	col = (unsigned char *)&color;
	return ((v4f){
		(float)(col[0]) / 255.0f,
		(float)(col[1]) / 255.0f,
		(float)(col[2]) / 255.0f,
		(float)(col[3]) / 255.0f});
}

static v4f			border_calc(v4f vec, mglrect rect)
{
	v2f				ws;
	v2f				rs;

	ws = (v2f){vec.x, vec.y};
	rs.x = rect.size.x * 0.5f;
	rs.y = rect.size.y * 0.5f;
	if (rect.border_left < 1.0f)
		vec.x = rect.border_left;
	else if (rect.border_left < rs.x)
		vec.x = rect.border_left / rect.size.x;
	else
		vec.x = 0.5f;
	if (rect.border_right < 1.0f)
		vec.z = rect.border_right;
	else if (rect.border_right < rs.x)
		vec.z = rect.border_right / rect.size.x;
	else
		vec.z = 0.5f;
	if (rect.border_bottom < 1.0f)
		vec.y = rect.border_bottom;
	else if (rect.border_bottom < rs.y)
		vec.y = rect.border_bottom / rect.size.y;
	else
		vec.y = 0.5f;
	if (rect.border_top < 1.0f)
		vec.w = rect.border_top;
	else if (rect.border_top < rs.y)
		vec.w = rect.border_top / rect.size.y;
	else
		vec.w = 0.5f;
	return (vec);
}

void				mgl_drawrect(mglwin *win, mglrect rect)
{
	static mglsha	shr;
	static GLuint	vbo = 0;
	GLint			loc;
	v4f				vec;

	if (!shr.id || !vbo)
		rect_init(&vbo, &shr);
	vec.z = (float)(win->data->screen_w);
	vec.w = (float)(win->data->screen_h);
	glUseProgram(shr.id);
	loc = glGetUniformLocation(shr.id, "position");
	if (rect.position.x < 1.0f)
		vec.x = (rect.position.x * vec.z * 2.0f) - 1.0f;
	else
		vec.x = ((rect.position.x / vec.z) * 2.0f) - 1.0f;
	if (rect.position.y < 1.0f)
		vec.y = 1.0f - (rect.position.y * vec.w * 2.0f);
	else
		vec.y = 1.0f - ((rect.position.y / vec.w) * 2.0f);
	glUniform2fv(loc, 1, (GLfloat *)&vec);
	loc = glGetUniformLocation(shr.id, "size");
	if (rect.size.x < 1.0f)
		vec.x = (rect.size.x *= vec.z) * 2.0f;
	else
		vec.x = (rect.size.x / vec.z) * 2.0f;
	if (rect.size.y < 1.0f)
		vec.y = (rect.size.y *= vec.w) * -2.0f;
	else
		vec.y = ((rect.size.y / vec.w) * -2.0f);
	glUniform2fv(loc, 1, (GLfloat *)&vec);
	loc = glGetUniformLocation(shr.id, "color");
	vec = coltov4f(rect.color);
	glUniform4fv(loc, 1, (GLfloat *)&vec);
	loc = glGetUniformLocation(shr.id, "border_color");
	vec = coltov4f(rect.border_color);
	glUniform4fv(loc, 1, (GLfloat *)&vec);
	loc = glGetUniformLocation(shr.id, "borders");
	vec = border_calc(vec, rect);
	glUniform4fv(loc, 1, (GLfloat *)&vec);
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glUseProgram(0);
}
