/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   basic.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/15 15:32:54 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 17:54:09 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASIC_SHADERS_H
#define BASIC_SHADERS_H

#include "mgl/shaders.h"

static const mglsha		drawimage2d_shader = (mglsha){
	.id = 0,
	.vshid = 0,
	.fshid = 0,
	.vsh =
#include "pixelbox.vert.h"
,
	.fsh =
#include "pixelbox.frag.h"
};

static const mglsha		mglstr2d_shader = (mglsha){
	.id = 0,
	.vshid = 0,
	.fshid = 0,
	.vsh =
#include "char.vert.h"
,
	.fsh =
#include "char.frag.h"
};

static const mglsha		mglrect2d_shader = (mglsha){
	.id = 0,
	.vshid = 0,
	.fshid = 0,
	.vsh =
#include "rect.vert.h"
,
	.fsh =
#include "rect.frag.h"
};

#endif
