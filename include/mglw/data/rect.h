/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rect.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 16:18:15 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 16:34:06 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RECT_H
#define RECT_H

# define MGL_SHADOWLEFT
# define MGL_SHADOWRIGHT
# define MGL_SHADOWTOP
# define MGL_SHADOWBOTTOM

typedef struct		mgl_rectangle
{
	unsigned long	flags;
	float			position[2];
	float			size[2];
	float			border_left;
	float			border_right;
	float			border_top;
	float			border_bottom;
	float			shadow[3];
	unsigned int	color;
	unsigned int	border_color;
	unsigned int	shadow_color;
}					mglrect;

#endif
