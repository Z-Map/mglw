/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/01 08:30:26 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/26 11:29:14 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_STRING_H
#define MGL_STRING_H

# include <string.h>

#include "mglw/data/charatlas.h"

typedef struct		mgl_string
{
	size_t			length;
	char			*str;
	mglca			*ca;
	unsigned int	vbo;
	unsigned int	vao;
}					mglstr;

#endif