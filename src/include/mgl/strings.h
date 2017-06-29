/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strings.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/29 05:02:29 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/29 18:43:50 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_STRINGS_H
# define MGL_STRINGS_H

#include "mglw_intern/window.h"

#include "mgl/data/vector.h"

# include "mgl/data/char.h"
# include "mgl/data/string.h"
# include "mgl/data/charatlas.h"

#include "mgl/drawer.h"

mglca		*mgl_charatlas_loadbuffer(mglca *ca);

void		mgl_drawcstr(mglca *ca_ascii, const char *str, v2f pos, float size);

#endif
