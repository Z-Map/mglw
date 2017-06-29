/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   charatlas.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/29 06:21:04 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/27 14:09:36 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_CHARATLAS_H
# define MGL_CHARATLAS_H

# include <string.h>

typedef struct		mgl_char_atlas
{
	unsigned int	*glyphs;
	float			*texoffset;
	float			*metrics;
	size_t			length;
	int				box[2];
	float			ascent;
	float			descent;
	float			linesize;
	unsigned int	texture;
	unsigned int	vbo;
	int				padding;
}					mglca;

#endif
