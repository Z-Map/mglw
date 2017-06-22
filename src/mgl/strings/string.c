/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/22 19:20:57 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/22 20:36:43 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "mgl/strings.h"

static void	*clean_exit(mglstr *ms, v4i *vid, int error)
{
	if (vid)
		free(vid);
	if (error)
	{
		if (ms->vbo)
			glDeleteBuffers(1, &(ms->vbo));
		if (ms->vao)
			glDeleteVertexArrays(1, &(ms->vao));
		if (ms)
			free(ms);
		return (NULL);
	}
	return (ms);
}

mglstr		*mgl_cstrtomglstr(mglca *charatlas, const char *str, float lsp)
{
	size_t	len = 0, i = 0;
	mglstr	*ms = NULL;
	v4i		*vid = NULL;
	float	*position = NULL;

	len = strlen(str);
	if (!len || !(ms = malloc((sizeof(char) * len) + sizeof(mglstr))))
		return (NULL);
	ms->vbo = 0;
	ms->vao = 0;
	ms->str = (char *)((size_t)ms + sizeof(mglstr));
	glGenBuffers(1, &(ms->vbo));
	glGenVertexArrays(1, &(ms->vao));
	if (!ms->vbo || !ms->vao
		|| !(vid = malloc((sizeof(v4i) + sizeof(float)) * len)))
		return (clean_exit(ms, vid, 1));
	position = (float *)(&vid[len]);
	while (i < len)
	{
		
	}
	return (ms);
}
