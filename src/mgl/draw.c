/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 17:30:03 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/27 15:02:25 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glload/gl_all.h"
#include "mgl/ressources/shaders/basic.h"
#include "mgl/shaders.h"
#include "mgl/drawer.h"

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
	glBufferData(GL_ARRAY_BUFFER, 12, vertx, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, *vbo);
	return (1);
}

static v4f			rgbatov4f(unsigned int color)
{
	unsigned char	*col;
	return ((v4f){
		(float)(col[0]) / 255.0f,
		(float)(col[1]) / 255.0f,
		(float)(col[2]) / 255.0f,
		(float)(col[3]) / 255.0f});
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
	vec.x = ((rect.position.x / vec.z) * 2.0f) - 1.0f;
	vec.y = 1.0f - ((rect.position.y / vec.w) * 2.0f);
	loc = glGetUniformLocation(shr.id, "position");
	glUniform2fv(loc, 1, &vec);
	loc = glGetUniformLocation(shr.id, "size");
	vec.x = ((rect.size.x / vec.z) * 2.0f) - 1.0f;
	vec.y = 1.0f - ((rect.size.y / vec.w) * 2.0f);
	glUniform2fv(loc, 1, &vec);
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glUseProgram(0);
}
