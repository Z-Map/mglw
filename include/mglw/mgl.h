/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mgl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 11:21:36 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/29 12:21:24 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_H
#define MGL_H

#include "mglw/mglw_header.h"

#include "mglw/window.h"

#include "mglw/data/charatlas.h"
#include "mglw/data/string.h"
#include "mglw/data/rect.h"

mglca		mgl_ttf_to_charatlas(const char *ttfpath, int *chartab, size_t len);

mglstr		*mgl_cstrtomglstr(mglca *charatlas, const char *str, float lsp,
				float space);
float		mgl_strpxlen(mglwin *win, mglstr *str, float size);
void		mgl_drawmglstr(mglwin *win, mglstr *str, float pos[2], float size,
				unsigned int color);
void		mgl_delstr(mglstr **str);

void		mgl_drawrect(mglwin *win, mglrect rect);

#endif
