/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/12/19 20:19:31 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/22 19:19:28 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MGL_SHADER_H
# define MGL_SHADER_H

# include "glload/gl_all.h"

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
