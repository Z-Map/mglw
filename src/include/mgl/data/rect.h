/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rect.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 16:18:15 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 16:32:56 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECT_H
#define RECT_H

#include "vector.h"

# define MGL_SHADOWLEFT
# define MGL_SHADOWRIGHT
# define MGL_SHADOWTOP
# define MGL_SHADOWBOTTOM

typedef struct		mgl_rectangle
{
	unsigned long	flags;
	v2f				position;
	v2f				size;
	float			border_left;
	float			border_right;
	float			border_top;
	float			border_bottom;
	v3f				shadow;
	unsigned int	color;
	unsigned int	border_color;
	unsigned int	shadow_color;
}					mglrect;

#endif
