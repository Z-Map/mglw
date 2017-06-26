/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mgl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/26 11:21:36 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 11:46:56 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_H
#define MGL_H

#include "mglw/mglw_header.h"

#include "mglw/window.h"

#include "mglw/data/charatlas.h"
#include "mglw/data/string.h"

mglca		mgl_ttf_to_charatlas(const char *ttfpath, int *chartab, size_t len);

mglstr		*mgl_cstrtomglstr(mglca *charatlas, const char *str, float lsp);
void		mgl_drawmglstr(mglwin *win, mglstr *str, float pos[2], float size,
				float color[4]);
void		mgl_delstr(mglstr **str);

#endif
