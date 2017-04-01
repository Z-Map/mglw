/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/19 20:19:31 by qloubier          #+#    #+#             */
/*   Updated: 2017/04/01 05:04:19 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_SHADER_H
#define MGL_SHADER_H

#include "glload/gl_2_1.h"

typedef struct				mgl_shader
{
	GLuint					id;
	GLuint					vshid;
	GLuint					fshid;
	unsigned int			padding;
	const char				*vsh;
	const char				*fsh;
}							mglsha;

# endif
