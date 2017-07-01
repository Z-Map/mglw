/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   font.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qloubier <qloubier@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/22 04:05:55 by qloubier          #+#    #+#             */
/*   Updated: 2017/07/01 13:01:07 by qloubier         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mglw/mglw.h"
#include "mgl/shaders.h"
#include "mgl/ressources/shaders/basic.h"
#include "mgl/ressources/quads.h"
#include "mgl/drawer.h"
#include "mgl/strings.h"

int		main()
{
	const struct timespec	t = (struct timespec){0, 120000000L};
	mglwin					*win;
	// mglimg					*img;
	// mgltex					*ping;
	mglca					*ca;
	mglstr					*str;
	mglrect					rect;
	// unsigned int			*tex;
	// unsigned int			x,y,i;

	if (!(mglw_init()) ||
		!(win = mglw_openwin(
			mglw_mkwin(MGLW_LEGACY_MODE, 0),
			800, 600, "Coucou !")))
		return (-1);
	mglw_setsetting(MGLWS_EXITKEY, MGLW_KEY_ESCAPE);
	mglw_limits(win, 500, 500, 800, 800);
	// img = (mglimg *)mglw_get2dlayer(win);
	// tex = (unsigned int *)img->pixels;
	// bzero(tex, img->memlen);
	//memcpy(img->pixels, pinguin->pixels, img->memlen);
	mglw_setGLContext(win);
	puts("coucou1");
	ca = mgl_ttf_to_charatlas("font.ttf", NULL, 0);
	str = mgl_cstrtomglstr(ca, "agylposiKL", 0.1f, 0.3f);
	// mglstr_auvoir(&str);
	puts("coucou2");
	// ca.texture = ping->texid;
	// for (y = 0; y < img->y; y++){
	// for (x = 0; x < img->x; x++)
	// {
	// 	i = x + (y * 1024);
	// 	tex[x + (y * img->x)] = 0xff << 24 | catex[i] |
	// 		(catex[i] << 16) | (catex[i] << 8);
	// }
	// }
	rect = (mglrect){0, {295.0f, 300.0f}, {310.f, mgl_strsize(str, 300.0f)},
		5.0f, 5.0f, 0.0f, 0.0f, {0.0f, 0.0f, 0.0f}, 0xFF1f356e, 0xFF0000FF, 0};
	puts("coucou3");
	while (mglwin_run(win))
	{
		mglw_setGLContext(win);
		mgl_drawrect(win, rect);
		mgl_drawmglstr(win, str, (float[2]){300,300}, mgl_strsize(str, 300.0f), 0xFFFFFFFF);
		nanosleep(&t, NULL);
	}
	mgl_delstr(&str);
	mgl_delcharatlas(&ca);
	mglw_close();
	return (0);
}
