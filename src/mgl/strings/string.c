/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/22 19:20:57 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/29 18:42:51 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "glload/gl_all.h"
#include "mgl/shaders.h"
#include "mgl/ressources/shaders/basic.h"
#include "mgl/strings.h"

static void		*clean_exit(mglstr *ms, v3f *vt, int error)
{
	if (vt)
		free(vt);
	if (error)
	{
		if (ms->vbo)
			glDeleteBuffers(1, &(ms->vbo));
		// if (ms->vao)
		// 	glDeleteVertexArrays(1, &(ms->vao));
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
		return ((size_t)((c > 32) ? c - 32 : 0));
	while (i < len)
	{
		if (ca->glyphs[i] == (unsigned int)c)
			return (i);
	}
	return (0);
}

mglstr			*mgl_cstrtomglstr(mglca *ca, const char *str, float lsp, float space)
{
	size_t		len = 0, i = 0, j = 0;
	int			h = 0;
	mglstr		*ms = NULL;
	v3f			*vt = NULL;
	v2f			*uvt = NULL;
	float		pos = 0.0f;
	v4f			vec;

	len = strlen(str);
	if (!len || !(ms = malloc((sizeof(char) * len)
		+ sizeof(mglstr))))
		return (NULL);
	ms->length = len;
	ms->vbo = 0;
	// ms->vao = 0;
	ms->ca = ca;
	ms->str = (char *)((size_t)ms + sizeof(mglstr));
	glGenBuffers(1, &(ms->vbo));
	// glGenVertexArrays(1, &(ms->vao));
	if (!(ms->vbo)
		|| !(vt = malloc((sizeof(v3f) + sizeof(v2f)) * 6 * len)))
		return (clean_exit(ms, vt, 1));
	uvt = (v2f *)(&vt[6 * len]);
	while (i < len)
	{
		if ((str[i] == ' ') || (str[i] == '\t'))
		{
			pos += (str[i] == '\t') ? (space * 4.0f) : space;
			i++;
			ms->length -= 1;
			continue;
		}
		j = find_charid(ca, str[i]);
		vec = (v4f){pos, ca->metrics[j].y, pos + ca->metrics[j].z, ca->metrics[j].y + ca->metrics[j].w};
		vt[h] = (v3f){vec.x, vec.y, 0.0};
		vt[h + 3] = vt[h + 1] = (v3f){vec.x, vec.w, 0.0};
		vt[h + 4] = vt[h + 2] = (v3f){vec.z, vec.y, 0.0};
		vt[h + 5] = (v3f){vec.z, vec.w, 0.0};
		// h += len * 6;
		uvt[h] = (v2f){ca->texoffset[j].x, ca->texoffset[j].y};
		uvt[h + 3] = uvt[h + 1] = (v2f){ca->texoffset[j].x, ca->texoffset[j].w};
		uvt[h + 4] = uvt[h + 2] = (v2f){ca->texoffset[j].z, ca->texoffset[j].y};
		uvt[h + 5] = (v2f){ca->texoffset[j].z, ca->texoffset[j].w};
		// uvt[h] = (v2f){0.0, 0.0};
		// uvt[h + 3] = uvt[h + 1] = (v2f){0.0, 1.0};
		// uvt[h + 4] = uvt[h + 2] = (v2f){1.0, 0.0};
		// uvt[h + 5] = (v2f){1.0, 1.0};
		pos += lsp + ca->metrics[j].z;
		i++;
		h += 6;
	}
	// glBindVertexArray(ms->vao);
	ms->pxlen = pos - lsp;
	if (ms->length < len)
		memmove(&(vt[h]), uvt, (sizeof(v2f) * h));
	glBindBuffer(GL_ARRAY_BUFFER, ms->vbo);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(v3f) + sizeof(v2f)) * 6 * ms->length,
		vt, GL_STATIC_DRAW);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
	// 	(void *)(len * 6 * sizeof(v3f)));
	// glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (clean_exit(ms, vt, 0));
}

float		mgl_strpxlen(mglstr *str, float size)
{
	size = size * ((float)str->ca->box.x / (float)str->ca->box.y);
	return (size * str->pxlen);
}

float		mgl_strsize(mglstr *str, float px)
{
	px /= str->pxlen;
	px *= ((float)str->ca->box.y / (float)str->ca->box.x);
	return (px);
}

void		mgl_drawmglstr(mglwin *win, mglstr *str, float pos[2], float size,
				unsigned int color)
{
	static mglsha	shr = (mglsha){ 0, 0, 0, 0, NULL, NULL};
	GLint			loc;
	int				sf, i = 94, tlvl = 0;
	v4f				col;

	if (!win || !str)
		return ;
	if (!shr.id)
	{
		shr = mglstr2d_shader;
		mgl_compile_shader(&shr);
		glUseProgram(shr.id);
		glBindAttribLocation(shr.id, 0, "v_pos");
		glBindAttribLocation(shr.id, 1, "v_uv");
		loc = glGetUniformLocation(shr.id, "chartex" );
		glUniform1i(loc, 0);
	}
	sf = round(size);
	glUseProgram(shr.id);
	pos[0] = ((pos[0] / (float)(win->data->screen_w)) * 2.0f) - 1.0f;
	pos[1] = 1.0f - (((pos[1] + size) / (float)(win->data->screen_h)) * 2.0f);
	loc = glGetUniformLocation(shr.id, "position");
	glUniform2fv(loc, 1, pos);
	loc = glGetUniformLocation(shr.id, "size");
	pos[1] = ((size / (float)(win->data->screen_h)) * 2.0f);
	size = size * ((float)str->ca->box.x / (float)str->ca->box.y);
	pos[0] = ((size / (float)(win->data->screen_w)) * 2.0f);
	glUniform2fv(loc, 1, pos);
	loc = glGetUniformLocation(shr.id, "color");
	col = coltov4f(color);
	glUniform4fv(loc, 1, (GLfloat *)&col);
	// loc = glGetUniformLocation(shr.id, "texlvl");
	while (((i /= 2) > sf) && (tlvl < 4))
		tlvl++;
	// glUniform1i(loc, tlvl);
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, str->vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void *)(str->length * 6 * sizeof(v3f)));
	glEnableVertexAttribArray(1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, str->ca->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, tlvl);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, tlvl);
	glDrawArrays(GL_TRIANGLES, 0, str->length * 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glUseProgram(0);
}
