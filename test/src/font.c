/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 04:05:55 by qloubier          #+#    #+#             */
/*   Updated: 2017/06/25 21:36:17 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mglw/mglw.h"
#include "mgl/shaders.h"
#include "mgl/ressources/shaders/basic.h"
#include "mgl/ressources/quads.h"
#include "mgl/strings.h"

int		main()
{
	const struct timespec	t = (struct timespec){0, 120000000L};
	mglwin					*win;
	// mglimg					*img;
	mgltex					*ping;
	mglca					ca;
	mglstr					*str;
	// unsigned int			*tex;
	// unsigned int			x,y,i;

	if (!(mglw_init()) ||
		!(win = mglw_openwin(
			mglw_mkwin(MGLW_LEGACY_MODE, 0),
			800, 600, "Coucou !")))
		return (-1);
	mglw_setsetting(MGLWS_EXITKEY, MGLW_KEY_ESCAPE);
	// img = (mglimg *)mglw_get2dlayer(win);
	// tex = (unsigned int *)img->pixels;
	// bzero(tex, img->memlen);
	ping = mglw_loadtex("cailloux.jpg");
	if (!ping)
		return (-1);
	//memcpy(img->pixels, pinguin->pixels, img->memlen);
	mglw_setGLContext(win);
	puts("coucou1");
	ca = mgl_ttf_to_charatlas("font.ttf", NULL, 0);
	str = mgl_cstrtomglstr(&ca, "Color : #FF00FF", 0.15f);
	// mglstr_auvoir(&str);
	puts("coucou2");
	glGenTextures(1, &(ping->texid));
	glBindTexture(GL_TEXTURE_2D, ping->texid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ping->cpuimage.x, ping->cpuimage.y,
		0, GL_RGB, ping->cpuimage.type, ping->cpuimage.pixels);
	glBindTexture(GL_TEXTURE_2D, 0);
	printf("%i\n", ca.texture);
	// ca.texture = ping->texid;
	// for (y = 0; y < img->y; y++){
	// for (x = 0; x < img->x; x++)
	// {
	// 	i = x + (y * 1024);
	// 	tex[x + (y * img->x)] = 0xff << 24 | catex[i] |
	// 		(catex[i] << 16) | (catex[i] << 8);
	// }
	// }
	static mglsha	shr = (mglsha){ 0, 0, 0, 0, NULL, NULL};
	GLint			loc;
	shr = drawimage2d_shader;
	mgl_compile_shader(&shr);
	glUseProgram(shr.id);
	glBindAttribLocation(shr.id, 0, "v_pos");
	glBindAttribLocation(shr.id, 1, "v_uv");
	glUseProgram(shr.id);
	loc = glGetUniformLocation(shr.id, "screen");
	glUniform2f(loc, (float)(win->data->screen_w), (float)(win->data->screen_h));
	loc = glGetUniformLocation(shr.id, "size");
	glUniform2f(loc, (float)(win->data->screen_w), (float)(win->data->screen_w));
	loc = glGetUniformLocation(shr.id, "position");
	glUniform2f(loc, 0.0, 0.0);
	puts("coucou3");
	while (mglwin_run(win))
	{
		mglw_setGLContext(win);
		// glUseProgram(shr.id);
		// glPushAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		// glEnable(GL_BLEND);
		// glEnable(GL_ALPHA_TEST);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glBindBuffer(GL_ARRAY_BUFFER, win->data->l2D_buffer);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 12));
		// glEnableVertexAttribArray(1);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, ca.texture);
		// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ping->cpuimage.x, ping->cpuimage.y, 0,
		// // 	ping->cpuimage.format, ping->cpuimage.type, ping->cpuimage.pixels);
		// glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		// glBindTexture(GL_TEXTURE_2D, 0);
		// glDisableVertexAttribArray(0);
		// glPopAttrib(GL_COLOR_BUFFER_BIT|GL_CURRENT_BIT);
		// glUseProgram(0);
		// mglw_draw_itow(win, (mglimg *)ping, 0, 0);
		mgl_drawmglstr(win, str, (v2f){1,1}, 46);
		nanosleep(&t, NULL);
	}
	mglw_close();
	return (0);
}
