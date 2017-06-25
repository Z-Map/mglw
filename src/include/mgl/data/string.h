/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/01 08:30:26 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/25 16:02:20 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_STRING_H
#define MGL_STRING_H

# include "glload/gl_all.h"
# include <string.h>

#include "charatlas.h"

typedef struct		mgl_string
{
	size_t			length;
	char			*str;
	mglca			*ca;
	GLuint			vbo;
	GLuint			vao;
}					mglstr;

#endif
