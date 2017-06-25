/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/22 19:20:57 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/25 14:47:18 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
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
		if (ms->vbo_data)
			glDeleteBuffers(1, &(ms->vbo_data));
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
	v3f			*vt = NULL;
	v2f			*uvt = NULL;
	float		pos = 0.0f;
	v4f			vec;

	len = strlen(str);
	printf("gui : %zi !\n", len);
	if (!len || !(ms = malloc((sizeof(char) * len)
		+ sizeof(mglstr))))
		return (NULL);
	ms->vbo_data = 0;
	ms->vao = 0;
	ms->ca = ca;
	ms->str = (char *)((size_t)ms + sizeof(mglstr));
	glGenBuffers(1, &(ms->vbo_data));
	// glGenVertexArrays(1, &(ms->vao));
	printf("gui %u - %u !\n", ms->vbo_data, ms->vao);
	if (!(ms->vbo_data)
		|| !(vt = malloc((sizeof(v3f) + sizeof(v2f)) * 6 * len)))
		return (clean_exit(ms, vt, 1));
	uvt = (v2f *)(&vt[6 * len]);
	printf("gui %zi %zi!\n", i, len);
	while (i < len)
	{
		j = find_charid(ca, str[i]);
		h = (int)i * 6;
		printf("char : '%c'\n", str[i]);
		printf("metrics : %.3f,%.3f - %.3f,%.3f\n", ca->metrics[j].x,
			ca->metrics[j].y, ca->metrics[j].z, ca->metrics[j].w);
		vec = (v4f){pos, ca->metrics[j].y, pos + ca->metrics[j].z, ca->metrics[j].y + ca->metrics[j].w};
		vt[h] = (v3f){vec.x, vec.y, 0.0};
		vt[h + 3] = vt[h + 1] = (v3f){vec.x, vec.w, 0.0};
		vt[h + 4] = vt[h + 2] = (v3f){vec.z, vec.y, 0.0};
		vt[h + 5] = (v3f){vec.z, vec.w, 0.0};
		printf("vec (%.2f,%.2f,%.2f,%.2f)\n", vec.x, vec.y, vec.z, vec.w);
		// h += len * 6;
		uvt[h] = (v2f){ca->texoffset[j].x, ca->texoffset[j].y};
		uvt[h + 3] = uvt[h + 1] = (v2f){ca->texoffset[j].z, ca->texoffset[j].w};
		uvt[h + 4] = uvt[h + 2] = (v2f){ca->texoffset[j].x, ca->texoffset[j].y};
		uvt[h + 5] = (v2f){ca->texoffset[j].z, ca->texoffset[j].w};
		pos += lsp + ca->metrics[j].z;
		printf("(%.2f,%.2f) (%.2f,%.2f) (%.2f,%.2f) (%.2f,%.2f)\n",
			vt[h].x, vt[h].y, vt[h + 1].x, vt[h + 1].y,
			vt[h + 2].x, vt[h + 2].y, vt[h + 5].x, vt[h + 5].y);
		i++;
	}
	// glBindVertexArray(ms->vao);
	glBindBuffer(GL_ARRAY_BUFFER, ms->vbo_data);
	glBufferData(GL_ARRAY_BUFFER, (sizeof(v3f) + sizeof(v2f)) * 6 * len,
		vt, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void *)(len * 6 * sizeof(v3f)));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	return (clean_exit(ms, vt, 0));
}

void		mgl_drawmglstr(mglwin *win, mglstr *str, v2f pos, float size)
{
	static mglsha	shr = (mglsha){ 0, 0, 0, 0, NULL, NULL};
	GLint			loc;

	if (!win || !str)
		return ;
	if (!shr.id)
	{
		shr = mglstr2d_shader;
		mgl_compile_shader(&shr);
		glBindAttribLocation(shr.id, 0, "v_pos");
		glBindAttribLocation(shr.id, 1, "v_uv");
		// glUniform1i(glGetUniformLocation(shr.id, "image" ), 0);
	}
	printf("gu !!\n");
	glUseProgram(shr.id);
	loc = glGetUniformLocation(shr.id, "screen");
	glUniform2f(loc, (float)(win->data->screen_w), (float)(win->data->screen_h));
	loc = glGetUniformLocation(shr.id, "size");
	glUniform1f(loc, (float)size);
	loc = glGetUniformLocation(shr.id, "position");
	glUniform2f(loc, pos.x, pos.y);
	loc = glGetUniformLocation(shr.id, "color");
	glUniform4f(loc, 1.0f, 1.0f, 1.0f, 1.0f);
	glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindBuffer(GL_ARRAY_BUFFER, str->vbo_data);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void *)(str->length * 6 * sizeof(v3f)));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, str->ca->texture);
	glDrawArrays(GL_TRIANGLES, 0, str->length * 6);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glPopAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
	glUseProgram(0);
}
