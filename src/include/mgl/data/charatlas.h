/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charatlas.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/29 06:21:04 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 10:13:22 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_CHARATLAS_H
# define MGL_CHARATLAS_H

# include "glload/gl_all.h"
# include "mgl/data/vector.h"

typedef struct		mgl_char_atlas
{
	unsigned int	*glyphs;
	v4f				*texoffset;
	v4f				*metrics;
	size_t			length;
	v2i				box;
	float			ascent;
	float			descent;
	float			linesize;
	GLuint			texture;
	GLuint			vbo;
	int				padding;
}					mglca;

#endif
