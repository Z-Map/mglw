/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 17:30:03 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 18:45:14 by qloubier         ###   ########.fr       */
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
	float			vertx[64] = {
		// central rect
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		0.0, 0.0,
		// border
		0.0, 0.0,
		1.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		-1.0, -1.0,
		2.0, -1.0,
		-1.0, 2.0,
		2.0, 2.0,
		// shadow
		-1.0, -1.0,
		2.0, -1.0,
		-1.0, 2.0,
		2.0, 2.0,
		-2.0, -2.0,
		3.0, -2.0,
		-2.0, 3.0,
		3.0, 3.0,
	};

	mgl_compile_shader(&shr);
	glGenBuffers(1, vbo);
}

void				mgl_drawrect(mglwin *win, mglrect rect);
{
	static mglsha	shr = mglrect2d_shader;
	static GLuint	vbo = 0;
	GLint			loc;


}
