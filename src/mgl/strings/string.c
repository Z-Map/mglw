/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/22 19:20:57 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/23 18:08:16 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "mgl/strings.h"

static void		*clean_exit(mglstr *ms, v4i *vid, int error)
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

static size_t	find_charid(mglca *ca, char c)
{
	size_t		len = ca->length, i = 0;

	if (!(ca->glyphs))
		return ((size_t)((c > 31) ? c - 31 : 0));
	while (i < len)
	{
		if (ca->glyphs[i] == (unsigned int)c)
			return (i);
	}
	return (0);
}

mglstr			*mgl_cstrtomglstr(mglca *ca, const char *str, float lsp)
{
	size_t		len = 0, i = 0, j = 0;
	int			h = 0;
	mglstr		*ms = NULL;
	v4i			*vid = NULL;
	float		pos = 0.0f;

	len = strlen(str);
	if (!len || !(ms = malloc(((sizeof(char) + sizeof(float)) * len)
		+ sizeof(mglstr))))
		return (NULL);
	ms->vbo = 0;
	ms->vao = 0;
	ms->str = (char *)((size_t)ms + sizeof(mglstr));
	ms->pos = (float *)((size_t)ms + sizeof(mglstr) + (sizeof(char) * len));
	glGenBuffers(1, &(ms->vbo));
	glGenVertexArrays(1, &(ms->vao));
	if (!ms->vbo || !ms->vao
		|| !(vid = malloc(sizeof(v4i) * len)))
		return (clean_exit(ms, vid, 1));
	while (i < len)
	{
		j = find_charid(ca, str[i]);
		h = (int)j * 4;
		vid[i] = (v4i){h, h + 1, h + 2, h + 3};
		ms->pos[i] = pos;
		pos += lsp + ca->metrics[j].z;
	}
	glBindVertexArray(ms->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ca->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void *)(ca->length * sizeof(v3f)));
    glEnableVertexAttribArray(1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ms->vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, len * sizeof(v4i), vid,
		GL_STATIC_DRAW);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (ms);
}
